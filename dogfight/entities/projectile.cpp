#include "projectile.h"

Projectile::Projectile(Scene* const s, Entity* const owner)
	: Entity(s), _owner(owner)
{
	b2FixtureDef fixtureDef;
	fixtureDef.filter.categoryBits = Physics::COLLISION_BULLET;
	fixtureDef.filter.maskBits = Physics::MASK_PROJECTILE;
	fixtureDef.isSensor = true;

	bulletComponent = addComponent<BulletComponent>(sf::Vector2f(10.0f, 10.0f), 2.0f, fixtureDef);

	shapeComponent = addComponent<ShapeComponent>();
	shapeComponent->setShape<sf::CircleShape>(10.0f);
	shapeComponent->getShape().setFillColor(sf::Color::Yellow);
	shapeComponent->getShape().setOrigin(sf::Vector2f(5.0f, 5.0f));
	shapeComponent->setLayer(1);
}

void Projectile::update(double dt)
{
	Entity::update(dt);
}

void Projectile::fire(sf::Vector2f direction)
{
	bulletComponent->teleport(_owner->getPosition());
	bulletComponent->fire(direction);
}

void Projectile::OnBeginOverlap(Entity* const e)
{
	if (e != _owner)
	{
		this->setForDelete();
	}
}
