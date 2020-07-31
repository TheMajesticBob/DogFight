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
	// shapeComponent->getShape().setRotation(-getRotation());

	Entity::render();
}

void Projectile::fire(std::string definition, Entity* const owner, sf::Vector2f position, const float& rotation)
{
	// Get correct projectile and shape definitions, setup owner
	_projectileDefiniton = Resources::get<defs::Projectile>(definition);
	std::shared_ptr<defs::GameShape> _projectileShape = Resources::get<defs::GameShape>(_projectileDefiniton->shape);
	_owner = static_cast<Ship*>(owner);
	
	// Setup physics fixture
	b2FixtureDef fixtureDef;
	fixtureDef.filter.categoryBits = Physics::COLLISION_BULLET;
	fixtureDef.filter.maskBits = Physics::MASK_PROJECTILE;
	fixtureDef.isSensor = true;
	fixtureDef.shape = _projectileShape->getPhysicsShape();

	SetTeam(owner->GetTeam());
	
	// Redefine bullet component with new data
	bulletComponent->redefine(_projectileDefiniton, fixtureDef);

	// Set our new shape
	sf::Color color = _owner->GetColor();
	sf::Color outlineColor = color;
	outlineColor.a = 127;

	shapeComponent->setShape<sf::Shape>(_projectileShape->getShape());
	shapeComponent->getShape().setFillColor(color);
	shapeComponent->getShape().setOutlineColor(outlineColor);
	shapeComponent->getShape().setOutlineThickness(0.75f);
	shapeComponent->setLayer(-1);

	// Set rotation
	// setRotation(-rotation);

	// Fire
	bulletComponent->fire(position, rotation);
	setAlive(true);
	setVisible(true);
}

void Projectile::OnBeginOverlap(Entity* const e)
{
	if (_alive && e != _owner)
	{
		Ship* ship = dynamic_cast<Ship*>(e);
		if (ship)
		{
			if (ship->GetTeam() != _owner->GetTeam())
			{
				ship->OnHit(_damage);
				Destroy();
			}
		}
		else 
		{
			Projectile* projectile = dynamic_cast<Projectile*>(e);
			if (projectile)
			{
				if (projectile->isAlive() && _projectileDefiniton->collideWithProjectiles && projectile->GetTeam() != GetTeam())
				{
					Destroy();
				}
			}
			else 
			{
				Destroy();
			}
		}
	}
}
