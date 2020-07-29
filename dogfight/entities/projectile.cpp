#include "projectile.h"
#include "../engine/game_resources.h"
#include "Ship.h"

Projectile::Projectile(Scene* const s)
	: Entity(s)
{
	bulletComponent = addComponent<BulletComponent>(2.0f);
	bulletComponent->teleport(sf::Vector2f(-100000000000000.0f, -100000000000000.0f));
	bulletComponent->setActive(false);

	sf::Vector2f _size(1,1);

	shapeComponent = addComponent<ShapeComponent>();
	shapeComponent->setShape<sf::RectangleShape>(_size);
}

void Projectile::Reset()
{
	Poolable::Reset();
}

void Projectile::Destroy()
{
	Poolable::Destroy();

	bulletComponent->teleport(sf::Vector2f(-100000000000000.0f, -100000000000000.0f));
	bulletComponent->setActive(false);

	setAlive(false);
	setVisible(false);
}

void Projectile::update(double dt)
{
	Entity::update(dt);
}

void Projectile::render()
{
	shapeComponent->getShape().setRotation(-getRotation());

	Entity::render();
}

void Projectile::fire(std::string definition, Entity* const owner, sf::Vector2f position, sf::Vector2f direction)
{
	_projectileDefiniton = Resources::get<defs::Projectile>(definition);
	std::shared_ptr<defs::GameShape> _projectileShape = Resources::get<defs::GameShape>(_projectileDefiniton->shape);
	_owner = owner;
	
	b2Vec2 collisionSize = Physics::sv2_to_bv2(_projectileDefiniton->size);

	b2PolygonShape shape;
	shape.SetAsBox(collisionSize.x, collisionSize.y);

	b2FixtureDef fixtureDef;
	fixtureDef.filter.categoryBits = Physics::COLLISION_BULLET;
	fixtureDef.filter.maskBits = Physics::MASK_PROJECTILE;
	fixtureDef.isSensor = true;
	fixtureDef.shape = &shape;
	
	bulletComponent->redefine(_projectileDefiniton, fixtureDef);


	shapeComponent->setShape<sf::Shape>(_projectileShape->getShape());
	shapeComponent->getShape().setFillColor(sf::Color::Yellow);
	shapeComponent->setLayer(1);

	setRotation(_owner->getRotation());

	//bulletComponent->teleport(position);
	bulletComponent->fire(position, direction);
	//bulletComponent->setActive(true);

	setAlive(true);
	setVisible(true);
}

void Projectile::OnBeginOverlap(Entity* const e)
{
	if (_alive && e != _owner)
	{
		Destroy();
		std::cout << "Projectile hit " << std::endl;
		
		Ship* ship = static_cast<Ship*>(e);
		if(ship)
		{
			ship->OnHit(_damage);
		}
	}
}
