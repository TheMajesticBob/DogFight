#include "shipPlayerController.h"
#include "../engine/game_resources.h"

using namespace sf;


ShipPlayerController::ShipPlayerController(Scene* const s, std::string controlscheme, Ship* ship) : PlayerController(s)
{
	//get input from json
	_controls = Resources::get<defs::Controls>(controlscheme);
	_ship = ship;
}

void ShipPlayerController::update(double dt)
{

	if (Keyboard::isKeyPressed((Keyboard::Key)_controls->accelerate))
	{
		_ship->Accelerate(1.0f);
	}
	if (Keyboard::isKeyPressed((Keyboard::Key)_controls->turnLeft))
	{
		_ship->Turn(-1.0f);
	}
	else if (Keyboard::isKeyPressed((Keyboard::Key)_controls->turnRight))
	{
		_ship->Turn(1.0f);
	}

	if (Keyboard::isKeyPressed((Keyboard::Key)_controls->shoot))
	{
		_ship->Fire();
	}

PlayerController::update(dt);
};
