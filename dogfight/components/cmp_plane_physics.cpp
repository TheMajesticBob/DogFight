#include "cmp_plane_physics.h"
#include "system_physics.h"
#include "system_renderer.h"
#include "system_resources.h"
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;
using namespace sf;
using namespace Physics;

void PlanePhysicsComponent::update(double dt)
{
	// If plane definition has not been initialised yet, don't do anything
	if(_planeDefinition == nullptr)
	{
		return;
	}

	// Reset acceleration for this frame
	_isAccelerating = false;

	// Get speed and normalised velocity
	auto velocity = _body->GetLinearVelocity();
	auto speed = velocity.Length();
	velocity.Normalize();

	// Draw debug stuff
	auto angle = _body->GetAngle() * 180.0f / M_PI;
	auto angleVector = b2Vec2(-cos(deg2rad(angle)), sin(deg2rad(angle)));

	_debugText.setString(
		"Angle: " + to_string(angle) + "\n" +
		"Angle vector: " + to_string(angleVector.x) + ", " + to_string(angleVector.y) + "\n" +
		"Velocity: " + to_string(velocity.x * speed) + ", " + to_string(velocity.y * speed) + "\n" +
		"Speed: " + to_string(speed)
	);

	// Clamp speed
	auto clampedSpeed = min(speed, _planeDefinition->maxSpeed);
	velocity *= clampedSpeed;
	_body->SetLinearVelocity(velocity);

	_debugShape.setRotation(angle);
	_debugShape.setPosition(_parent->getPosition());

	PhysicsComponent::update(dt);
}

void PlanePhysicsComponent::render()
{
	UI::queue(&_debugText);

	if(_debugDraw)
	{
		Renderer::queue(&_debugShape);
	}
}

void PlanePhysicsComponent::accelerate(float value)
{
	// If plane definition has not been initialised yet, don't do anything
	if(_planeDefinition == nullptr)
	{
		return;
	}

	// Clamp value between 0 and 1
	value = max(0.0f, min(value, 1.0f));

	_isAccelerating = true;

	// double angle = _body->GetAngle() * 180.0f / M_PI;
	forceApplied = _planeDefinition->acceleration * sv2_to_bv2(getForwardVector(), false); // b2Vec2(-cos(deg2rad(angle)), sin(deg2rad(angle)));

	_body->ApplyForce(forceApplied, _body->GetPosition(), true);
}

void PlanePhysicsComponent::turn(float value)
{
	// If plane definition has not been initialised yet, don't do anything
	if(_planeDefinition == nullptr)
	{
		return;
	}

	// Clamp value between -1 and 1
	value = -max(-1.0f, min(value, 1.0f));

	auto accelerationMultiplier = _isAccelerating ? _planeDefinition->accelerationRotationMultiplier : 1.0f;
	_body->SetAngularVelocity(_body->GetAngularVelocity() + deg2rad(value * accelerationMultiplier * _planeDefinition->rotationSpeed));
}

sf::Vector2f PlanePhysicsComponent::getForwardVector()
{
	double angle = _body->GetAngle() * 180.0f / M_PI;
	return normalize(sf::Vector2f(-cos(deg2rad(angle)), -sin(deg2rad(angle))));
}

PlanePhysicsComponent::PlanePhysicsComponent(Entity* p,
											 const Vector2f& size, std::shared_ptr<defs::Plane> definition, b2FixtureDef& fixtureDef)
	: PhysicsComponent(p, true, size, fixtureDef)
{
	_planeDefinition = definition;

	_size = sv2_to_bv2(size, true);

	_body->SetSleepingAllowed(false);
	_body->SetFixedRotation(false);
	_body->SetBullet(true);

	b2Fixture* fixtures = _body->GetFixtureList();
	_body->DestroyFixture(fixtures);

	// Define triangle points
	sf::Vector2f points[] = {
		{0.0f, -0.5f * size.x},
		{0.0f, 0.5f * size.x},
		{-sqrt(3.0f) / 2.0f * size.y, 0.0f}
	};

	// Set debug shape points
	_debugShape.setPointCount(3);
	for( int i = 0; i < 3; ++i )
	{
		_debugShape.setPoint(i, points[i]);
	}

    // Create the fixture shape
    b2PolygonShape Shape;
	// Set triangle vertices
	b2Vec2 vertices[3];
	for(int i = 0; i < 3; ++i )
	{
		// We have to scale the points properly by using sv2_to_bv2
		b2Vec2 point = sv2_to_bv2(points[i]);
		vertices[i].Set(point.x, point.y);
	}
	Shape.Set(vertices, 3);

    b2FixtureDef FixtureDef;
    // Fixture properties
    //FixtureDef.density = _dynamic ? 10.f : 0.f;
    FixtureDef.friction = _dynamic ? 0.1f : 0.8f;
    FixtureDef.restitution = .2;
    FixtureDef.shape = &Shape;

    // Add to body
    _fixture = _body->CreateFixture(&FixtureDef);

	_maxSpeed = _planeDefinition->maxSpeed;
	_rotationSpeed = _planeDefinition->rotationSpeed;
	_body->SetAngularDamping(_planeDefinition->angularDamping);
	_body->SetLinearDamping(_planeDefinition->linearDamping);

	// Debug
  	_font = Resources::get<sf::Font>("RobotoMono-Regular.ttf");
  	_debugText.setFont(*_font);
	_debugText.setPosition({20.0f, 20.0f});

	_body->SetTransform(_body->GetPosition(), 90.0f / 180.0f * M_PI);
}