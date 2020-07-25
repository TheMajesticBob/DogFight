#pragma once
#include <ecm.h>
//#include "../components/cmp_plane_physics.h"
//#include "../components/cmp_decision_tree.h"
//#include "../components/cmp_state_machine.h"
//#include "../components/cmp_sprite.h"
#include "player.h"
// #include <SFML/Graphics.hpp>

class Enemy : public Entity
{
	public:
		Enemy() = delete;
		Enemy(Scene* const s);

		void update(double) override;
		std::shared_ptr<PlanePhysicsComponent> GetMovementComponent() { return movementComponent; }

		void Accelerate(float Value);
		void Turn(float Value);
		void Fire();

protected:
	std::shared_ptr<PlanePhysicsComponent> movementComponent;
	std::shared_ptr<ShapeComponent> shapeComponent;
	std::shared_ptr<ShapeComponent> thrusterComponent;

	std::shared_ptr<class BasicBehaviourTree> _behaviourTree;
	std::shared_ptr<class EnemyBlackboard> _blackboard;
};