#pragma once
#include <system_controller.h>
#include "../entities/Ship.h"
#include "../definitions.h"


class ShipPlayerController : public PlayerController 
{
protected:
	std::shared_ptr<defs::Controls> _controls;
	std::shared_ptr<class TextComponent> _textComponent;
	Ship* _ship;
	bool _isFiring = false;

public:
	ShipPlayerController() = delete;
	ShipPlayerController(Scene* const s, std::string controlscheme, Ship* ship);
	~ShipPlayerController();

	virtual void OnPossess(std::shared_ptr<Pawn> pawn) override;

	void OnShipDestroyed(std::shared_ptr<Ship> ship);

	void Accelerate(float value);
	void Turn(float value);
	void StartFiring();
	void StopFiring();

protected:

	virtual void update(double dt);
	void tryRestart();
	void goToMainMenu();
	void OnPausePressed();
};