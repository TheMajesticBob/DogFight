#include "behaviour_tree.h"

void BehaviourTree::setCurrentNode(BTNode* node)
{
	_currentNode = node;
	std::cout << "Active node: " << _currentNode->getName() << std::endl;
}

BehaviourTree::BehaviourTree(Scene* const s, std::shared_ptr<Blackboard> b)
	: Entity(s), _blackboard(b)
{
	_rootNode = makeNode<RootNode>(nullptr);
}

void BehaviourTree::startBehaviourTree()
{
	if (_rootNode != nullptr)
	{
		_rootNode->run();
	}
}

// Root node does not go into its child?

void BehaviourTree::update(double dt)
{
	if (_currentNode != nullptr)
	{
		_currentNode->update(dt);
	}
}
