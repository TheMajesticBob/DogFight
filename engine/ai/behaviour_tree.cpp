#include "behaviour_tree.h"
#include "../engine/system_resources.h"
#include "../engine/system_renderer.h"

void BehaviourTree::setCurrentNode(BTNode* node)
{
	_currentNode = node;
}

BehaviourTree::BehaviourTree(Scene* const s, std::shared_ptr<Blackboard> b)
	: Entity(s), _blackboard(b)
{
	_rootNode = makeNode<RootNode>(nullptr);

	// Debug
	_font = Resources::get<sf::Font>("RobotoMono-Regular.ttf");
	_debugText.setFont(*_font);
	_debugText.setCharacterSize(16);
}

void BehaviourTree::startBehaviourTree(Entity* controlledEntity)
{
	_controlledEntity = controlledEntity;

	// Start with the root node
	if (_rootNode != nullptr)
	{
		_rootNode->run(controlledEntity);
	}
}

void BehaviourTree::update(double dt)
{
	if (_controlledEntity == nullptr)
	{
		return;
	}

	// Update all decorators
	for (auto decorator : _decorators)
	{
		if (decorator != nullptr)
		{
			decorator->update(dt, _controlledEntity);
		}
	}

	// Then update current node
	if (_currentNode != nullptr)
	{
		_currentNode->update(dt, _controlledEntity);

		_debugText.setString(_currentNode->getName());
		_debugText.setPosition(_controlledEntity->getPosition() - sf::Vector2f(_debugText.getLocalBounds().width / 2.0f, -30.0f));

		if (_currentNode == _rootNode.get())
		{
			_currentNode->run(_controlledEntity);
		}
	}
}

void BehaviourTree::render()
{
	Entity::render();

	// Renderer::queue(&_debugText);
}
