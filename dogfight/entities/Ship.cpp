#include "ship.h"
#include <engine.h>
#include "projectile.h"
#include "system_physics.h"
#include "../components/cmp_particlesystem.h"
#include "../components/cmp_health_component.h"
#include "../components/cmp_weapon.h"
#include "../engine/game_resources.h"

using namespace sf;

Ship::Ship(Scene* const s, std::string shipDefinition) : Pawn(s)
{
	// Get the ship definition
	_shipDefinition = Resources::get<defs::Plane>(shipDefinition);

	// Size?
	Vector2f size = { 30.0f, 30.0f };

	// Setup collision filters for the entity
	b2FixtureDef FixtureDef;
	FixtureDef.filter.categoryBits = Physics::COLLISION_DYNAMIC;
	FixtureDef.filter.maskBits = Physics::MASK_DYNAMIC;

	// Setup ship movement, health and weapon components based on ship definition
	movementComponent = addComponent<PlanePhysicsComponent>(size, _shipDefinition, FixtureDef);
	healthComponent = addComponent<HealthComponent>(_shipDefinition->health);
	for (auto& w : _shipDefinition->weapons)
	{
		weaponComponents.push_back(addComponent<WeaponComponent>(w));
	}

	// Thruster Particle System
	thrusterPS = addComponent<ThrusterParticleSystem>();

	// Thruster shape lol its ugly af. TODO: Replace this shiiiit
	thrusterComponent = addComponent<ShapeComponent>();
	thrusterComponent->setShape<RectangleShape>(Vector2f(size.x, size.y / 2.0f));
	thrusterComponent->getShape().setFillColor(Color::Yellow);
	thrusterComponent->getShape().setOrigin(Vector2f(size.x / 2.0f, size.y / 4.0f));
	thrusterComponent->setVisibility(false);
	thrusterComponent->setLayer(-1);

	shapeComponent = addComponent<ShapeComponent>();
	shapeComponent->setShape<sf::ConvexShape>(4);
	sf::ConvexShape* shape = (sf::ConvexShape*)&shapeComponent->getShape(); 
	shape->setPoint(0, { 0.0f, -size.x / 2.0f });
	shape->setPoint(1, { -size.x * sqrt(3.0f) / 2.0f, 0.0f });
	shape->setPoint(2, { 0.0f, size.x / 2.0f });
	shape->setPoint(3, { -size.x * sqrt(3.0f) / 12.0f, 0.0f });
	shape->setFillColor(sf::Color::Black);
	shape->setOutlineColor(sf::Color::White);
	shape->setOutlineThickness(2.0f);

	// Setup draw shape
	// Right vertex
	drawVerts[0] = 0.0f;
	drawVerts[1] = -size.x / 2.0f;

	// Top vertex
	drawVerts[2] = -size.x * sqrt(3) / 2.0f;
	drawVerts[3] = 0; // -1.0f * Physics::physics_scale;

	// Left vertex
	drawVerts[4] = 0;
	drawVerts[5] = size.x / 2.0f;

	// Bottom vertex
	drawVerts[6] = -size.x * sqrt(3) / 12.0f;
	drawVerts[7] = 0;

	for (int i = 0; i < 8; ++i)
	{
		drawVerts[i] = drawVerts[i];
	}

	// Currently this is how we draw the ship
	movementComponent->setDebugDraw(false);

	// The following does not match the actual triangle shape of movement component, therefore I left the drawing to it
	// We should definitely look into that when we get assets
// 	float radius = size.x / 2.0f;
// 
// 	shapeComponent = addComponent<ShapeComponent>();
// 	shapeComponent->setShape<CircleShape>(radius, 30);
// 	shapeComponent->getShape().setFillColor(Color::Blue);
// 	shapeComponent->getShape().setOrigin(Vector2f(0.0f, 0.0f));
}

void Ship::update(double dt)
{
	// Update thruster visibility based on whether the ship is accelerating
	// TODO: Replace with particle system

	thrusterPS->setOffset(getPosition() - movementComponent->getForwardVector());

	if (movementComponent->isAccelerating())
	{
		thrusterPS->fire();
	}

	Pawn::update(dt);
}

void Ship::render()
{
	Pawn::render();
}

void Ship::OnHit(float damage)
{
	// Pass the damage to health component
	healthComponent->onHit(damage);

	// Destroy ship when at 0hp
	if (healthComponent->getCurrentHealth() <= 0.0f)
	{
		OnDestroyed();
		setForDelete();
	}
}

void Ship::OnDestroyed()
{
	// TODO: Make Boom!
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
}