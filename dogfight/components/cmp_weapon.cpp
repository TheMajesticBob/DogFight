#include "cmp_weapon.h"
#include "engine.h"
#include "../entities/projectile.h"
#include "../engine/game_resources.h"

void WeaponComponent::update(double dt)
{
	if (_fireCooldown > 0.0f)
	{
		_fireCooldown -= dt;
	}
}

WeaponComponent::WeaponComponent(Entity* p, std::string weaponDefinition) : Component(p)
{
	_weaponDefinition = Resources::get<defs::Weapon>(weaponDefinition);
}

void WeaponComponent::fire(sf::Vector2f directionVector)
{
	if (_fireCooldown <= 0.0f)
	{
		for (int i = 0; i < _weaponDefinition->projectilesPerRound; ++i)
		{
			auto projectile = _parent->scene->makeEntity<Projectile>(_weaponDefinition->projectile, _parent);
			projectile->setDamage(_weaponDefinition->damage);
			projectile->fire(directionVector);
		}
		
		_fireCooldown = 1.0f / _weaponDefinition->rateOfFire;
	}
}
