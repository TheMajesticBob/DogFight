#include "cmp_bullet.h"
#include "system_physics.h"
#include "system_resources.h"
#include "../engine/game_resources.h"

using namespace std;
using namespace sf;
using namespace Physics;

void BulletComponent::fire(Vector2f& direction)
{
	direction = normalize(direction);
	_body->SetLinearVelocity(_initialSpeed*sv2_to_bv2(direction));
}

void BulletComponent::update(double dt) 
{
	// If projectile definition has not been initialised yet, don't do anything
	if (_projectileDefinition == nullptr)
	{
		return;
	}

	_lifetime -= (float)dt;
	if (_lifetime <= 0.f) 
	{
		_parent->setForDelete();
	}

	PhysicsComponent::update(dt);
}

BulletComponent::BulletComponent(Entity* p, const Vector2f& size, float lifetime, b2FixtureDef& fixtureDef)
	: PhysicsComponent(p, true, size, fixtureDef), _lifetime(lifetime)
{
	_size = sv2_to_bv2(size, true);

	_body->SetSleepingAllowed(false);
	_body->SetFixedRotation(false);
	_body->SetBullet(true);
	
	_projectileDefinition = Resources::get<defs::Projectile>("test");

	_initialSpeed = _projectileDefinition->initialSpeed;
	_affectedByGravity = _projectileDefinition->gravityAffects;
	_body->SetLinearDamping(_projectileDefinition->linearDamping);
	_body->SetGravityScale(_projectileDefinition->gravityAffects ? 1.0f : 0.0f);
	setMass(_projectileDefinition->mass);
}
