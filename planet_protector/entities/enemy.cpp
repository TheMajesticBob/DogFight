#include "enemy.h"
#include <engine.h>
#include "system_physics.h"

#include "../ai/ai_basicbehaviourtree.h"

Enemy::Enemy(Scene* const s, std::string shipDefinition)
	: Ship(s, shipDefinition)
{
	// Create a blackboard and set required variables
	_blackboard = std::make_shared<Blackboard>();
	_blackboard->setEntity("MyPawn", this);
	auto playerArray = s->ents.find("Player");
	if (playerArray.size() > 0)
	{
		_blackboard->setEntity("TargetEntity", playerArray[0].get()); // TODO: Create a task/service to automatically look for targets
	}

	// Create and run the behaviour tree
	_behaviourTree = s->makeEntity<BasicBehaviourTree>(_blackboard);
	_behaviourTree->startBehaviourTree(this);

	SetColor(sf::Color(255, 0, 0));
}

void Enemy::update(double dt)
{
	Ship::update(dt);
}

void Enemy::OnDestroyed()
{
	_behaviourTree->setForDelete();
}