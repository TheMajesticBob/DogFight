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
							ShipPlayerController,	 						// in class Ship
							&ShipPlayerController::StartFiring				// take a pointer to Ship::StartFiring method
							>(this));						// and pass in a pointer to the instance (our controlled ship in this case)

	InputHandler::BindKey(_controls->shoot, Event::KeyReleased, FKeyDelegate::from_function<ShipPlayerController, &ShipPlayerController::StopFiring>(this));
	InputHandler::BindAxis(_controls->accelerate, 1.0f, FAxisDelegate::from_function<ShipPlayerController, &ShipPlayerController::Accelerate>(this));
	InputHandler::BindAxis(_controls->turnLeft, -1.0f, FAxisDelegate::from_function<ShipPlayerController, &ShipPlayerController::Turn>(this));
	InputHandler::BindAxis(_controls->turnRight, 1.0f, FAxisDelegate::from_function<ShipPlayerController, &ShipPlayerController::Turn>(this));

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

	// Unbind keyboard events
	InputHandler::UnbindKey(_controls->shoot, Event::KeyPressed,FKeyDelegate::from_function<ShipPlayerController, &ShipPlayerController::StartFiring>(this));
	InputHandler::UnbindKey(_controls->shoot, Event::KeyReleased, FKeyDelegate::from_function<ShipPlayerController, &ShipPlayerController::StopFiring>(this));
	InputHandler::UnbindAxis(_controls->accelerate, FAxisDelegate::from_function<ShipPlayerController, &ShipPlayerController::Accelerate>(this));
	InputHandler::UnbindAxis(_controls->turnLeft, FAxisDelegate::from_function<ShipPlayerController, &ShipPlayerController::Turn>(this));
	InputHandler::UnbindAxis(_controls->turnRight, FAxisDelegate::from_function<ShipPlayerController, &ShipPlayerController::Turn>(this));
}

void ShipPlayerController::OnPossess(std::shared_ptr<Pawn> pawn)
{
	//_ship = std::dynamic_pointer_cast<Ship>(pawn);
	Controller::OnPossess(pawn);
}

void ShipPlayerController::OnShipDestroyed(std::shared_ptr<Ship> ship)
{
	_ship = nullptr;
}

void ShipPlayerController::Accelerate(float value)
{
	if (_ship != nullptr)
	{
		_ship->Accelerate(value);
	}
}

void ShipPlayerController::Turn(float value)
{
	if (_ship != nullptr)
	{
		_ship->Turn(value);
	}
}

void ShipPlayerController::StartFiring()
{
	if (_ship != nullptr)
	{
		_ship->StartFiring();
	}
}

void ShipPlayerController::StopFiring()
{
	if (_ship != nullptr)
	{
		_ship->StopFiring();
	}
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

}

void ShipPlayerController::goToMainMenu()
{
	Engine::ChangeScene(&menu, true);
}
