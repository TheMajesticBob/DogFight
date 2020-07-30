#include "projectile.h"
#include "../engine/game_resources.h"
#include "Ship.h"

Projectile::Projectile(Scene* const s)
	: Entity(s)
{
	bulletComponent = addComponent<BulletComponent>(2.0f);
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
	// Get correct projectile and shape definitions, setup owner
	_projectileDefiniton = Resources::get<defs::Projectile>(definition);
	std::shared_ptr<defs::GameShape> _projectileShape = Resources::get<defs::GameShape>(_projectileDefiniton->shape);
	_owner = owner;
	
	// Setup physics fixture
	b2FixtureDef fixtureDef;
	fixtureDef.filter.categoryBits = Physics::COLLISION_BULLET;
	fixtureDef.filter.maskBits = Physics::MASK_PROJECTILE;
	fixtureDef.isSensor = true;
	fixtureDef.shape = _projectileShape->getPhysicsShape();
	
	// Redefine bullet component with new data
	bulletComponent->redefine(_projectileDefiniton, fixtureDef);

	// Set our new shape
	shapeComponent->setShape<sf::Shape>(_projectileShape->getShape());
	shapeComponent->getShape().setFillColor(sf::Color::Yellow);
	shapeComponent->setLayer(1);

	// Set rotation
	setRotation(_owner->getRotation());

	// Fire
	bulletComponent->fire(position, direction);
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
