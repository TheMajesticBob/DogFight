#pragma once
#include <ecm.h>

class Controller : public Entity {

public:
	//controlled pawn
	virtual void update(double dt);

};

class PlayerController : public Controller {

public:

	virtual void update(double dt);
	
};

class AIController : public Controller {

public:

	virtual void update(double dt);
};