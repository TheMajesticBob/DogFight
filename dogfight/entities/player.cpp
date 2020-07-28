#include "player.h"

Player::Player(Scene* const s, std::string shipDefinition) : Ship(s, shipDefinition)
{
	// Required for the AI to find players
	addTag("Player");
	color = sf::Color(0.0f, 1.0f, 0.0f);
}

void Player::update(double dt)
{
	Ship::update(dt);
}