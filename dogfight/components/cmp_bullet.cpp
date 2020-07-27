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
	//impulse(sf::rotate(Vector2f(0, 15.f), -_parent->getRotation()));
	_body->SetLinearVelocity(_initialSpeed*sv2_to_bv2(direction));
	
		//   s->getShape().setFillColor(Color::Red);
		//   s->getShape().setOrigin(8.f, 8.f);
		//   auto p = bullet->addComponent<PhysicsComponent>(true, Vector2f(8.f, 8.f));
		//   p->setRestitution(.4f);
		//   p->setFriction(.005f);
		//   p->impulse(sf::rotate(Vector2f(0, 15.f), -_parent->getRotation()));
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

	_parent->setPosition(invert_height(bv2_to_sv2(_body->GetPosition())));
	//PhysicsComponent::update(dt);
}

BulletComponent::BulletComponent(Entity* p, std::shared_ptr<defs::Projectile> definition, float lifetime, b2FixtureDef& fixtureDef)
	: PhysicsComponent(p, true, definition->size, fixtureDef), _lifetime(lifetime)
{
	_projectileDefinition = definition;

	_body->SetSleepingAllowed(false);
	_body->SetFixedRotation(false);
	_body->SetBullet(true);

	_initialSpeed = _projectileDefinition->initialSpeed;
	_affectedByGravity = _projectileDefinition->gravityAffects;
	_body->SetLinearDamping(_projectileDefinition->linearDamping);
	_body->SetGravityScale(_projectileDefinition->gravityAffects ? 1.0f : 0.0f);
	setMass(_projectileDefinition->mass);
}
