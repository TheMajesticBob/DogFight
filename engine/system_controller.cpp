#include "system_controller.h"

void Controller::update(double dt)
{
	Entity::update(dt);
}

void PlayerController::update(double dt)
{
	Controller::update(dt);
}

void AIController::update(double dt)
{
	Controller::update(dt);
}
