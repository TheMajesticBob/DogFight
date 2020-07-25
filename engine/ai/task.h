#pragma once
#include "behaviour_tree.h"

class BTTask : public BTNode
{
public:
	BTTask(BehaviourTree* const bt, std::string name) : BTNode(bt, name) { }

protected:
	std::shared_ptr<Entity> _myEntity;
};