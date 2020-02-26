#include "cmp_plane_physics.h"
#include "system_physics.h"
#include "system_renderer.h"
#include "system_resources.h"
#include <cmath>

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

	PhysicsComponent::update(dt);
}

void PlanePhysicsComponent::render() { Renderer::queue(&_debugText); }

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
}