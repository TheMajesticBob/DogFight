#pragma once
#include <ecm.h>


class Controller : public Entity {

public:
	
	//controlled pawn
	Controller() = delete;
	Controller(Scene* const s) : Entity(s) {}
	virtual void update(double dt);

	virtual void OnPossess(std::shared_ptr<Pawn> possessedPawn) {}
	virtual void OnUnpossess(std::shared_ptr<Pawn> unpossessedPawn) {}

	void possess(std::shared_ptr<Pawn> pawn) 
	{
		_possessedPawn = pawn;
		_possessedPawn->ptrControl = this; 

		OnPossess(pawn);
	}

	void unpossess() 
	{ 
		_possessedPawn->ptrControl = nullptr; 
		OnUnpossess(_possessedPawn);
		_possessedPawn = nullptr; 
	}

private:
	std::shared_ptr<Pawn> _possessedPawn;

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