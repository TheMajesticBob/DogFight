#pragma once
#include <system_controller.h>
#include "../entities/Ship.h"
#include "../definitions.h"


class ShipPlayerController : PlayerController {
protected:
	std::shared_ptr<defs::Controls> _controls;
	Ship* _ship;
	
public:
	ShipPlayerController() = delete;
	ShipPlayerController(Scene* const s, std::string controlscheme, Ship* ship);
	
	virtual void update(double dt);
};