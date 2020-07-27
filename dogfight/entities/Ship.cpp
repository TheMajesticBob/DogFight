#include "Ship.h"
#include <engine.h>
#include "projectile.h"
#include "system_physics.h"
#include "../components/cmp_health_component.h"
#include "../components/cmp_weapon.h"
#include "../engine/game_resources.h"

using namespace sf;

Ship::Ship(Scene* const s, std::string shipDefinition) : Pawn(s)
{
	_shipDefinition = Resources::get<defs::Plane>(shipDefinition);

	Vector2f size = { 30.0f, 30.0f };

	b2FixtureDef FixtureDef;
	FixtureDef.filter.categoryBits = Physics::COLLISION_DYNAMIC;
	FixtureDef.filter.maskBits = Physics::MASK_DYNAMIC;

	movementComponent = addComponent<PlanePhysicsComponent>(size, _shipDefinition, FixtureDef);
	movementComponent->setDebugDraw(true);

	healthComponent = addComponent<HealthComponent>(_shipDefinition->health);

	for (auto& w : _shipDefinition->weapons)
	{
		weaponComponents.push_back(addComponent<WeaponComponent>(w));
	}

	thrusterComponent = addComponent<ShapeComponent>();
	thrusterComponent->setShape<RectangleShape>(Vector2f(size.x, size.y / 2.0f));
	thrusterComponent->getShape().setFillColor(Color::Yellow);
	thrusterComponent->getShape().setOrigin(Vector2f(size.x / 2.0f, size.y / 4.0f));
	thrusterComponent->setVisibility(false);
	thrusterComponent->setLayer(-1);

	// The following does not match the actual triangle shape of movement component, therefore I left the drawing to it
	// We should definitely look into that when we get assets

	// shapeComponent = addComponent<ShapeComponent>();
	// shapeComponent->setShape<CircleShape>(size.x / 2.0f, 3);
	// shapeComponent->getShape().setFillColor(Color::Blue);
	// shapeComponent->getShape().setOrigin({15.0f, 10.0f});
	// shapeComponent->getShape().setRotation(45.0f);
}

void Ship::update(double dt)
{
	if (fireCooldown > 0.0f)
	{
		fireCooldown -= dt;
	}
}

void Ship::OnHit(float damage)
{
	healthComponent->onHit(damage);
	if (healthComponent->getCurrentHealth() <= 0.0f)
	{
		OnDestroyed();
		setForDelete();
	}
}

void Ship::OnDestroyed()
{
	//TODO: Make Boom!
}

void Ship::Accelerate(float Value)
{
	movementComponent->accelerate(Value);
}

void Ship::Turn(float Value)
{
	movementComponent->turn(Value);
}

void Ship::Fire()
{
	for (auto weapon : weaponComponents)
	{
		if (weapon)
		{
			weapon->fire(movementComponent->getForwardVector());
		}
	}
// 	if (fireCooldown <= 0.0f)
// 	{
// 		auto projectile = scene->makeEntity<Projectile>("test", this);
// 		projectile->fire();
// 		fireCooldown = 1.0f / fireRate;
// 	}
}

Player::Player(Scene* const s, std::string shipDefinition) : Ship(s, shipDefinition)
{
	addTag("Player");
}

void Player::update(double dt)
{
	Ship::update(dt);

	if (Keyboard::isKeyPressed(Keyboard::W))
	{
		Accelerate(1.0f);
	}
	if(Keyboard::isKeyPressed(Keyboard::A))
	{
		Turn(-1.0f);
	}
	else if (Keyboard::isKeyPressed(Keyboard::D))
	{
		Turn(1.0f);
	}

	if(Keyboard::isKeyPressed(Keyboard::P))
	{
		// movementComponent->setDebugDraw(!movementComponent->getDebugDraw());
	}

	if (Keyboard::isKeyPressed(Keyboard::Space))
	{
		Fire();
	}

	thrusterComponent->setVisibility(movementComponent->isAccelerating());
	Pawn::update(dt);
}