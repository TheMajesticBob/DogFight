#pragma once
#include <ecm.h>
//#include "../components/cmp_plane_physics.h"
//#include "../components/cmp_decision_tree.h"
//#include "../components/cmp_state_machine.h"
//#include "../components/cmp_sprite.h"
#include "Ship.h"
// #include <SFML/Graphics.hpp>

class Enemy : public Ship
{
	public:
		Enemy() = delete;
		Enemy(Scene* const s, std::string shipDefinition);

		void update(double) override;
		void OnDestroyed() override;

protected:
	std::shared_ptr<class BasicBehaviourTree> _behaviourTree;
	std::shared_ptr<class Blackboard> _blackboard;
};