#pragma once
#include <ecm.h>
#include "../components/cmp_basic_movement.h"
#include "../components/cmp_sprite.h"

class Player : public Entity
{
	public:
		Player() = delete;
		Player(Scene* const s);

	protected:
		std::shared_ptr<BasicMovementComponent> movementComponent;
		std::shared_ptr<ShapeComponent> shapeComponent;
};