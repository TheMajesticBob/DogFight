#pragma once
#include <system_controller.h>
#include "../entities/Ship.h"
#include "../definitions.h"


class ShipPlayerController : public PlayerController {
protected:
	std::shared_ptr<defs::Controls> _controls;
	std::shared_ptr<class TextComponent> _textComponent;
	Ship* _ship;
	
public:
	ShipPlayerController() = delete;
	ShipPlayerController(Scene* const s, std::string controlscheme, Ship* ship);
	~ShipPlayerController();

	virtual void update(double dt);
	void tryRestart();
	void goToMainMenu();
};