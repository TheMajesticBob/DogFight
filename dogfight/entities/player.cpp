#include "player.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <engine.h>
#include "projectile.h"
#include "system_physics.h"

using namespace sf;

Player::Player(Scene* const s) : Pawn(s)
{
	Vector2f size = { 30.0f, 30.0f };

	b2FixtureDef FixtureDef;
	FixtureDef.filter.categoryBits = Physics::COLLISION_PLAYER;
	FixtureDef.filter.maskBits = Physics::MASK_PLAYER;

	movementComponent = addComponent<PlanePhysicsComponent>(size, FixtureDef);
	movementComponent->setDebugDraw(true);

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

void Player::update(double dt)
{
	if (fireCooldown > 0.0f)
	{
		fireCooldown -= dt;
	}

	if (Keyboard::isKeyPressed(Keyboard::W))
	{
		movementComponent->accelerate(1.0f);
		thrusterComponent->setVisibility(true);
	}
	else
	{
		thrusterComponent->setVisibility(false);
	}

	if(Keyboard::isKeyPressed(Keyboard::A))
	{
		movementComponent->turn(-1.0f);
	}
	else if (Keyboard::isKeyPressed(Keyboard::D))
	{
		movementComponent->turn(1.0f);
	}

	if(Keyboard::isKeyPressed(Keyboard::P))
	{
		// movementComponent->setDebugDraw(!movementComponent->getDebugDraw());
	}

	if (Keyboard::isKeyPressed(Keyboard::Space) && fireCooldown <= 0.0f)
	{
		auto projectile = scene->makeEntity<Projectile>(this);
		projectile->fire(movementComponent->getAngle());
		fireCooldown = 1.0f / fireRate;
	}

	Pawn::update(dt);
}