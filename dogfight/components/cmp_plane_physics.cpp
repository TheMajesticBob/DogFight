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
	auto clampedSpeed = min(speed, _maxSpeed);
	velocity *= clampedSpeed;
	_body->SetLinearVelocity(velocity);

	_debugShape.setRotation(angle);
	_debugShape.setPosition(_parent->getPosition());

	PhysicsComponent::update(dt);
}

void PlanePhysicsComponent::render()
{
	Renderer::queue(&_debugText);
	Renderer::queue(&_debugShape);
}

void PlanePhysicsComponent::accelerate()
{
	_isAccelerating = true;

	double angle = _body->GetAngle() * 180.0f / M_PI;
	forceApplied = _acceleration * b2Vec2(-cos(deg2rad(angle)), sin(deg2rad(angle)));

	_body->ApplyForce(forceApplied, _body->GetPosition(), true);
}

void PlanePhysicsComponent::turn(float value)
{
	auto accelerationMultiplier = _isAccelerating ? _accelerationRotationMultiplier : 1.0f;
	_body->SetAngularVelocity(_body->GetAngularVelocity() + deg2rad(value * accelerationMultiplier * _rotationSpeed));
}

PlanePhysicsComponent::PlanePhysicsComponent(Entity* p,
											 const Vector2f& size)
	: PhysicsComponent(p, true, size)
{
	_size = sv2_to_bv2(size, true);

	_body->SetSleepingAllowed(false);
	_body->SetFixedRotation(false);
	_body->SetBullet(true);

	b2Fixture* fixtures = _body->GetFixtureList();
	_body->DestroyFixture(fixtures);

	_debugShape.setPointCount(3);
	_debugShape.setPoint(1, {0.0f, -0.5f * size.x});
	_debugShape.setPoint(2, {0.0f, 0.5f * size.x});
	_debugShape.setPoint(0, {-sqrt(3.0f) / 2.0f * size.y, 0.0f});

    // Create the fixture shape
    b2PolygonShape Shape;
    // SetAsBox box takes HALF-Widths!
	b2Vec2 vertices[3];
	vertices[0].Set(sqrt(3.0f) / 2.0f * size.y, 0.0f);
	vertices[1].Set(0.0f, -0.5f * _size.x);
	vertices[2].Set(0.0f, 0.5f * _size.x);
	Shape.Set(vertices, 3);
    //Shape.SetAsBox(sv2_to_bv2(size).x * 0.5f, sv2_to_bv2(size).y * 0.5f);
    b2FixtureDef FixtureDef;
    // Fixture properties
    // FixtureDef.density = _dynamic ? 10.f : 0.f;
    FixtureDef.friction = _dynamic ? 0.1f : 0.8f;
    FixtureDef.restitution = .2;
    FixtureDef.shape = &Shape;
    // Add to body
    _fixture = _body->CreateFixture(&FixtureDef);
    //_fixture->SetRestitution(.9)
    FixtureDef.restitution = .2;

	//TODO: Replace with JSON initialisation
	_maxSpeed = 30.0f;
	_rotationSpeed = 2.6f;
	_acceleration = 40.0f;
	_accelerationRotationMultiplier = 0.45f;
	_body->SetAngularDamping(10.0f);
	_body->SetLinearDamping(0.1f);

	// Debug
  	_font = Resources::get<sf::Font>("RobotoMono-Regular.ttf");
  	_debugText.setFont(*_font);
	_debugText.setPosition({20.0f, 20.0f});

	_body->SetTransform(_body->GetPosition(), 90.0f / 180.0f * M_PI);
}