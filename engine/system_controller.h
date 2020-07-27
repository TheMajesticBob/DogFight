#pragma once
#include <ecm.h>


class Controller : public Entity {

public:
	
	//controlled pawn
	Controller() = delete;
	Controller(Scene* const s) : Entity(s) {}
	virtual void update(double dt);

};

class PlayerController : public Controller {

public:
	PlayerController() = delete;
	PlayerController(Scene* const s) : Controller(s){}

	virtual void update(double dt);
	
};

class AIController : public Controller {

public:

	virtual void update(double dt);
};