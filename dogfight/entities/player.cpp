#include "player.h"
#include <SFML/Graphics.hpp>

Player::Player(Scene* const s) : Entity(s)
{
	movementComponent = addComponent<BasicMovementComponent>();
	shapeComponent = addComponent<ShapeComponent>();
	shapeComponent->setShape<sf::CircleShape>(10.0f);
	shapeComponent->getShape().setFillColor(sf::Color::Red);
}