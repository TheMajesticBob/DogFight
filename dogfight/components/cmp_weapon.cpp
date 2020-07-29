#include "cmp_weapon.h"
#include "engine.h"
#include "../entities/projectile.h"
#include "../engine/game_resources.h"

void WeaponComponent::update(double dt)
{
	_shape->setPosition(_parent->getPosition() + rotate(_relativePosition, -_parent->getRotation()));
	_shape->setRotation(-_parent->getRotation());

	if (_fireCooldown > 0.0f)
	{
		_fireCooldown -= dt;
	}
}

void WeaponComponent::render()
{
	ShapeComponent::render();
}

WeaponComponent::WeaponComponent(Entity* p, defs::WeaponSlot& weaponSlotDefinition) : ShapeComponent(p)
{
	_weaponDefinition = Resources::get<defs::Weapon>(weaponSlotDefinition.weaponName);
	_relativePosition = weaponSlotDefinition.relativePosition;

	std::shared_ptr<defs::GameShape> _weaponShape = Resources::get<defs::GameShape>(_weaponDefinition->shape);
	setShape<sf::Shape>(_weaponShape->getShape());
	
	setLayer(-1);
}

void WeaponComponent::fire(sf::Vector2f directionVector)
{
	if (_fireCooldown <= 0.0f)
	{
		for (int i = 0; i < _weaponDefinition->projectilesPerRound; ++i)
		{
			auto projectile = Pool<Projectile>::Get(); // _parent->scene->makeEntity<Projectile>();
			if (projectile)
			{
				projectile->setDamage(_weaponDefinition->damage);
				projectile->fire(_weaponDefinition->projectile, _parent, _parent->getPosition() + rotate(_relativePosition, -_parent->getRotation()), directionVector);
			}
 		}
		
		_fireCooldown = 1.0f / _weaponDefinition->rateOfFire;
	}
}
