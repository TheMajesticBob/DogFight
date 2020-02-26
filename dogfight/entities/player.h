#pragma once
#include <ecm.h>
#include "../components/cmp_plane_physics.h"
#include "../components/cmp_sprite.h"

class Player : public Entity
{
	public:
		Player() = delete;
		Player(Scene* const s);

		void update(double) override;
		std::shared_ptr<PlanePhysicsComponent> GetMovementComponent() { return movementComponent; }

	protected:
		std::shared_ptr<PlanePhysicsComponent> movementComponent;
		std::shared_ptr<ShapeComponent> shapeComponent;
};