#include "player.h"

Player::Player(Scene* const s, std::string shipDefinition) : Ship(s, shipDefinition)
{
	// Required for the AI to find players
	addTag("Player");
}

void Player::update(double dt)
{
	// Controls. TODO: Move the code to a PlayerController, use dynamic keybindings
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		Accelerate(1.0f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		Turn(-1.0f);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		Turn(1.0f);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
	{
		// movementComponent->setDebugDraw(!movementComponent->getDebugDraw());
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		Fire();
	}

	Ship::update(dt);
}