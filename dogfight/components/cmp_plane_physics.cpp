#include "cmp_plane_physics.h"
#include "system_physics.h"
#include "system_renderer.h"
#include "system_resources.h"
#include "../engine/game_resources.h"
#include "system_ui.h"
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

	PhysicsComponent::update(dt);
}

void PlanePhysicsComponent::render()
{
	UI::queue(&_debugText);
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

	// Apply force
	_isAccelerating = true;
	forceApplied = _planeDefinition->acceleration * sv2_to_bv2(getForwardVector(), false);
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
	float torqueValue = 1.0f * deg2rad(value * accelerationMultiplier * _planeDefinition->rotationSpeed);
	_body->ApplyAngularImpulse(torqueValue, true); // SetAngularVelocity(_body->GetAngularVelocity() + deg2rad(value * accelerationMultiplier * _planeDefinition->rotationSpeed));
}

sf::Vector2f PlanePhysicsComponent::getForwardVector()
{
	double angle = _body->GetAngle() * 180.0f / M_PI;
	return normalize(sf::Vector2f(-cos(deg2rad(angle)), -sin(deg2rad(angle))));
}

PlanePhysicsComponent::PlanePhysicsComponent(Entity* p, std::shared_ptr<defs::Ship> definition, b2FixtureDef& fixtureDef)
	: PhysicsComponent(p, true)
{
	_planeDefinition = definition;
	std::shared_ptr<defs::GameShape> _shapeDefinition = Resources::get<defs::GameShape>(_planeDefinition->shape);

	b2FixtureDef FixtureDef;
	// Fixture properties
	FixtureDef.density = _dynamic ? 10.f : 0.f;
	FixtureDef.friction = _dynamic ? 0.1f : 0.8f;
	FixtureDef.restitution = .2;
	FixtureDef.shape = _shapeDefinition->getPhysicsShape();

	b2BodyDef BodyDef;
	// Is Dynamic(moving), or static(Stationary)
	BodyDef.type = _dynamic ? b2_dynamicBody : b2_staticBody;
	BodyDef.position = sv2_to_bv2(invert_height(p->getPosition()));

	// Create the body
	_body = Physics::GetWorld()->CreateBody(&BodyDef);
	_body->SetActive(true);
	_body->SetUserData(p);
	_body->SetSleepingAllowed(false);
	_body->SetFixedRotation(false);
	_body->SetBullet(true);

	_desiredActive = true;

    // Add to body
    _fixture = _body->CreateFixture(&FixtureDef);

	// Setup ship properties
	_maxSpeed = _planeDefinition->maxSpeed;
	_rotationSpeed = _planeDefinition->rotationSpeed;
	_body->SetAngularDamping(_planeDefinition->angularDamping);
	_body->SetLinearDamping(_planeDefinition->linearDamping);

	// Debug
  	_font = Resources::get<sf::Font>("RobotoMono-Regular.ttf");
  	_debugText.setFont(*_font);
	_debugText.setPosition({20.0f, 20.0f});
}