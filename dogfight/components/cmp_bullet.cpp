#include "cmp_bullet.h"
#include "system_physics.h"
#include "system_resources.h"
#include "../engine/game_resources.h"
#include "../entities/projectile.h"

using namespace std;
using namespace sf;
using namespace Physics;

void BulletComponent::fire(const Vector2f& position, const float& rotation)
{
	// direction = normalize(direction);

	float32 angleRadians = deg2rad(rotation);

	_body->SetTransform(sv2_to_bv2(invert_height(position)), angleRadians);
	_body->SetLinearVelocity(_initialSpeed*sv2_to_bv2(-sf::Vector2f(cos(angleRadians),sin(angleRadians))));
	_body->SetAngularVelocity(_projectileDefinition->initialRotation);
	setActive(true);
}

void BulletComponent::redefine(std::shared_ptr<defs::Projectile> definition, b2FixtureDef& fixtureDef)
{
	_projectileDefinition = definition;
	
	if (_fixture)
	{
		_body->DestroyFixture(_fixture);
		_fixture = _body->CreateFixture(&fixtureDef);
	}
	
	_lifetime = _projectileDefinition->lifetime;
	_initialSpeed = _projectileDefinition->initialSpeed;
	_affectedByGravity = _projectileDefinition->gravityAffects;
	_body->SetFixedRotation(true);
	_body->SetLinearDamping(_projectileDefinition->linearDamping);
	_body->SetGravityScale(_projectileDefinition->gravityAffects ? 1.0f : 0.0f);
	setMass(_projectileDefinition->mass);
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
		Projectile* parentProjectile = static_cast<Projectile*>(_parent);
		parentProjectile->Destroy();
	}

	// _parent->setPosition(invert_height(bv2_to_sv2(_body->GetPosition())));
	PhysicsComponent::update(dt);
}

BulletComponent::BulletComponent(Entity* p, float lifetime, b2FixtureDef& fixtureDef)
	: PhysicsComponent(p, true, b2CircleShape(), fixtureDef), _lifetime(lifetime)
{
	_body->SetSleepingAllowed(false);
	_body->SetFixedRotation(false);
	_body->SetBullet(true);
}
