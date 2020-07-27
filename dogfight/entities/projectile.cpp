#include "projectile.h"
#include "../engine/game_resources.h"
#include "Ship.h"

Projectile::Projectile(Scene* const s, std::string definition, Entity* const owner)
	: Entity(s), _owner(owner)
{
	b2FixtureDef fixtureDef;
	fixtureDef.filter.categoryBits = Physics::COLLISION_BULLET;
	fixtureDef.filter.maskBits = Physics::MASK_PROJECTILE;
	fixtureDef.isSensor = true;

	_projectileDefiniton = Resources::get<defs::Projectile>(definition);

	bulletComponent = addComponent<BulletComponent>(_projectileDefiniton, 2.0f, fixtureDef);

	shapeComponent = addComponent<ShapeComponent>();
	shapeComponent->setShape<sf::RectangleShape>(_projectileDefiniton->size);
	shapeComponent->getShape().setFillColor(sf::Color::Yellow);
	shapeComponent->getShape().setOrigin(_projectileDefiniton->size / 2.0f);
	shapeComponent->setLayer(1);
}

void Projectile::update(double dt)
{
	Entity::update(dt);
}

void Projectile::fire(sf::Vector2f direction)
{
	setRotation(_owner->getRotation());
	bulletComponent->teleport(_owner->getPosition());
	bulletComponent->fire(direction);
}

void Projectile::OnBeginOverlap(Entity* const e)
{
	if (e != _owner)
	{
		this->setForDelete();
		Ship* ship = static_cast<Ship*>(e);
		if(ship)
		{
			ship->OnHit(_damage);
		}
	}
}
