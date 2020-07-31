#include "shipPlayerController.h"
#include "../engine/game_resources.h"
#include "../planet_protector/game.h"
#include <input_handler.h>
#include <engine.h>

using namespace sf;


ShipPlayerController::ShipPlayerController(Scene* const s, std::string controlscheme, Ship* ship) : PlayerController(s)
{
	//get input from json
	_controls = Resources::get<defs::Controls>(controlscheme);
	_ship = ship;

	// Bind keyboard events
	InputHandler::BindKey(	_controls->shoot,				// Keycode to bind to
							Event::KeyPressed,				// KeyPressed or KeyReleased, we don't handle any other kinds
							FKeyDelegate::from_function<	// Create delegate to a member method
							Ship,	 						// in class Ship
							&Ship::StartFiring				// take a pointer to Ship::StartFiring method
							>(_ship));						// and pass in a pointer to the instance (our controlled ship in this case)

	InputHandler::BindKey(_controls->shoot, Event::KeyReleased, FKeyDelegate::from_function<Ship, &Ship::StopFiring>(_ship));
	InputHandler::BindAxis(_controls->accelerate, 1.0f, FAxisDelegate::from_function<Ship, &Ship::Accelerate>(_ship));
	InputHandler::BindAxis(_controls->turnLeft, -1.0f, FAxisDelegate::from_function<Ship, &Ship::Turn>(_ship));
	InputHandler::BindAxis(_controls->turnRight, 1.0f, FAxisDelegate::from_function<Ship, &Ship::Turn>(_ship));

	InputHandler::BindKey(sf::Keyboard::Space, Event::KeyPressed, FKeyDelegate::from_function<ShipPlayerController, &ShipPlayerController::tryRestart>(this));
	InputHandler::BindKey(sf::Keyboard::Escape, Event::KeyPressed, FKeyDelegate::from_function<ShipPlayerController, &ShipPlayerController::goToMainMenu>(this));

// 	_textComponent = addComponent<TextComponent>();
// 	_textComponent->setDrawOnUI(true);
// 	_textComponent->SetText("Game over\nPress space to restart\nPress escape to go back to main menu");
// 	_textComponent->setVisibility(false);
// 	_textComponent->getText()->setPosition(sf::Vector2f(Engine::getWindowSize()) / 2.0f);
// 	_textComponent->getText()->setOrigin(_textComponent->getText()->getLocalBounds().getSize() / 2.0f);
}

ShipPlayerController::~ShipPlayerController()
{
	InputHandler::UnbindKey(sf::Keyboard::Space, Event::KeyPressed, FKeyDelegate::from_function<ShipPlayerController, &ShipPlayerController::tryRestart>(this));
	InputHandler::UnbindKey(sf::Keyboard::Escape, Event::KeyPressed, FKeyDelegate::from_function<ShipPlayerController, &ShipPlayerController::goToMainMenu>(this));

	InputHandler::UnbindKey(_controls->shoot, Event::KeyReleased, FKeyDelegate::from_function<Ship, &Ship::StopFiring>(_ship));
	InputHandler::UnbindAxis(_controls->accelerate, FAxisDelegate::from_function<Ship, &Ship::Accelerate>(_ship));
	InputHandler::UnbindAxis(_controls->turnLeft, FAxisDelegate::from_function<Ship, &Ship::Turn>(_ship));
	InputHandler::UnbindAxis(_controls->turnRight, FAxisDelegate::from_function<Ship, &Ship::Turn>(_ship));
}

void ShipPlayerController::update(double dt)
{
// 	if (!_ship || !_ship->isAlive())
// 	{
// 		_textComponent->setVisibility(true);
// 	}
// 	else 
// 	{
// 		_textComponent->setVisibility(false);
// 	}
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

void ShipPlayerController::tryRestart()
{
	if (!_ship || !_ship->isAlive())
	{
		Engine::ChangeScene(&testing, true);
	}
}

void ShipPlayerController::goToMainMenu()
{
	Engine::ChangeScene(&menu, true);
}
