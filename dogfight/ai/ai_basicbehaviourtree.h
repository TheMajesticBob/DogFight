#pragma once
#include "ai/behaviour_tree.h"
#include "ai_testingtask.h"

class BasicBehaviourTree : public BehaviourTree
{
public:
	BasicBehaviourTree(Scene* const s, std::shared_ptr<Blackboard> b = nullptr)
		: BehaviourTree(s, b)
	{
		std::shared_ptr<BTT_MoveTowardsEntity> moveTowardsPlayer = makeNode<BTT_MoveTowardsEntity>("Move Towards Player");
		moveTowardsPlayer->MinThrottleDistance = 600.0f;
		moveTowardsPlayer->MinTurnAngle = 20.0f;

		std::shared_ptr<BTT_FireAtEntity> fireAtEntity = makeNode<BTT_FireAtEntity>("Fire at Player", 1.0f);
		std::shared_ptr<BTD_IsTargetInFront> raycastDecorator = makeDecorator<BTD_IsTargetInFront>("Is Target in Front?");
		std::shared_ptr<BTD_Cooldown> cooldownDecorator = makeDecorator<BTD_Cooldown>("Fire Cooldown", 1.5f);
		fireAtEntity->addDecorator(raycastDecorator);
		fireAtEntity->addDecorator(cooldownDecorator);

		std::shared_ptr<BTD_IsAlive> isTargetAlive = makeDecorator<BTD_IsAlive>("Is Target Alive?");
		isTargetAlive->key = "TargetEntity";

		std::shared_ptr<BTSelector> selector = makeNode<BTSelector>("Selector",
			std::vector<std::shared_ptr<BTNode>>
			{
				std::static_pointer_cast<BTNode>(fireAtEntity),
				std::static_pointer_cast<BTNode>(moveTowardsPlayer),
			});

		selector->addDecorator(isTargetAlive);
		getRootNode()->setChild(selector);
	}
};