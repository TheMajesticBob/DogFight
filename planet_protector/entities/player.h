#pragma once
#include "ship.h"

class Player : public Ship
{
public:
	Player() = delete;
	Player(Scene* const s, std::string shipDefinition);
	~Player();

	void update(double) override;
};