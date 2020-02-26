#include "player.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>

using namespace sf;

Player::Player(Scene* const s) : Entity(s)
{
	Vector2f size = { 30.0f, 30.0f };

	movementComponent = addComponent<PlanePhysicsComponent>(size);
	//movementComponent->
	shapeComponent = addComponent<ShapeComponent>();
	shapeComponent->setShape<RectangleShape>(size);
	shapeComponent->getShape().setFillColor(Color::Red);
	shapeComponent->getShape().setOrigin(size / 2.0f);
}

void Player::update(double dt)
{
	if(Keyboard::isKeyPressed(Keyboard::W))
	{
		movementComponent->accelerate();
	}

	if(Keyboard::isKeyPressed(Keyboard::A))
	{
		movementComponent->turn(-1.0f);
	}
	else if (Keyboard::isKeyPressed(Keyboard::D))
	{
		movementComponent->turn(1.0f);
	}


	Entity::update(dt);
}