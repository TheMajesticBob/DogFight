#include "shipPlayerController.h"
#include "../engine/game_resources.h"
#include "input_handler.h"

using namespace sf;


ShipPlayerController::ShipPlayerController(Scene* const s, std::string controlscheme, Ship* ship) : PlayerController(s)
{
	//get input from json
	_controls = Resources::get<defs::Controls>(controlscheme);
	_ship = ship;

	InputHandler::BindKey(_controls->shoot, Event::KeyPressed, FKeyDelegate::from_function<Ship, &Ship::StartFiring>(_ship));
	InputHandler::BindKey(_controls->shoot, Event::KeyReleased, FKeyDelegate::from_function<Ship, &Ship::StopFiring>(_ship));
	InputHandler::BindAxis(_controls->accelerate, 1.0f, FAxisDelegate::from_function<Ship, &Ship::Accelerate>(_ship));
	InputHandler::BindAxis(_controls->turnLeft, -1.0f, FAxisDelegate::from_function<Ship, &Ship::Turn>(_ship));
	InputHandler::BindAxis(_controls->turnRight, 1.0f, FAxisDelegate::from_function<Ship, &Ship::Turn>(_ship));
}

void ShipPlayerController::update(double dt)
{

// 	if (Keyboard::isKeyPressed((Keyboard::Key)_controls->accelerate))
// 	{
// 		_ship->Accelerate(1.0f);
// 	}
// 	if (Keyboard::isKeyPressed((Keyboard::Key)_controls->turnLeft))
// 	{
// 		_ship->Turn(-1.0f);
// 	}
// 	else if (Keyboard::isKeyPressed((Keyboard::Key)_controls->turnRight))
// 	{
// 		_ship->Turn(1.0f);
// 	}
// 
// 	if (Keyboard::isKeyPressed((Keyboard::Key)_controls->shoot))
// 	{
// 		// _ship->Fire();
// 	}

PlayerController::update(dt);
};
