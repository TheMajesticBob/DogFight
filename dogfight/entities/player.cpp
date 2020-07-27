#include "player.h"

Player::Player(Scene* const s, std::string shipDefinition) : Ship(s, shipDefinition)
{
	// Required for the AI to find players
	addTag("Player");
}

void Player::update(double dt)
{
	Ship::update(dt);
}