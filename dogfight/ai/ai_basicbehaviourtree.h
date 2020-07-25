#pragma once
#include "ai/behaviour_tree.h"
#include "ai_testingtask.h"

class EnemyBlackboard : public Blackboard
{
public:
	class Enemy* myPawn;
	std::shared_ptr<Entity> targetEntity;
};

class BasicBehaviourTree : public BehaviourTree
{
public:
	BasicBehaviourTree(Scene* const s, std::shared_ptr<Blackboard> b = nullptr)
		: BehaviourTree(s, b)
	{
		std::shared_ptr<BTT_MoveTowardsEntity> moveTowardsPlayer = makeNode<BTT_MoveTowardsEntity>("Move Towards Player");

		std::shared_ptr<BTSelector> selector = makeNode<BTSelector>("Selector",
			std::vector<std::shared_ptr<BTNode>>
			{
				std::static_pointer_cast<BTNode>(moveTowardsPlayer),
			});

		getRootNode()->setChild(selector);
	}
};