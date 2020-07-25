#pragma once
#include "behaviour_tree.h"

class BTTask : public BTNode
{
public:
	BTTask(BehaviourTree* const bt, std::string name) : BTNode(bt, name) {}
};