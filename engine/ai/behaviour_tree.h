#pragma once
#include <vector>
#include <memory>
#include <map>
#include <ecm.h>

// Blackboard is a data storage class for AI tasks, residing within the BehaviourTree
class Blackboard
{
public:
	inline void setBool(std::string key, bool value) { boolMap[key] = value; }
	inline bool getBool(std::string key) { return boolMap[key]; }

	inline void setString(std::string key, std::string value) { stringMap[key] = value; }
	inline std::string getString(std::string key) { return stringMap[key]; }

	inline void setFloat(std::string key, float value) { floatMap[key] = value; }
	inline float getFloat(std::string key) { return floatMap[key]; }

	inline void setInt(std::string key, int value) { intMap[key] = value; }
	inline int getInt(std::string key) { return intMap[key]; }

	inline void setEntity(std::string key, Entity* value) { entityMap[key] = value; }
	inline Entity* getEntity(std::string key) { return entityMap[key]; }

private:
	std::map<std::string, bool> boolMap;
	std::map<std::string, std::string> stringMap;
	std::map<std::string, float> floatMap;
	std::map<std::string, int> intMap;
	std::map<std::string, Entity*> entityMap;

};

// BehaviourTree class manages the AI tree
class BehaviourTree : public Entity, public std::enable_shared_from_this<BehaviourTree>
{
private:
	std::shared_ptr<class RootNode> _rootNode;
	std::shared_ptr<class Blackboard> _blackboard;
	class BTNode* _currentNode;

	void setCurrentNode(BTNode* node);

public:
	BehaviourTree() = delete;
	BehaviourTree(Scene* const s, std::shared_ptr<class Blackboard> b);

	const std::shared_ptr<class Blackboard> getBlackboard() { return _blackboard; }
	const std::shared_ptr<class RootNode> getRootNode() { return _rootNode; }

	void startBehaviourTree();
	virtual void update(double dt) override;

	// Create nodes
	template<typename T, typename... Targs>
	std::shared_ptr<T> makeNode(Targs... params)
	{
		static_assert(std::is_base_of<BTNode, T>::value, "T != BTNode");
		auto newNode = std::make_shared<T>(this, params...);
		return newNode;
	}

	// Create decorators
	template<typename T, typename... Targs>
	std::shared_ptr<T> makeDecorator(Targs... params)
	{
		static_assert(std::is_base_of<BTDecorator, T>::value, "T != BTDecorator");
		auto newDecorator = std::make_shared<T>(this, params...);
		return newDecorator;
	}

protected:
	friend class BTNode;
	friend class BTDecorator;
};


// Decorators are used as conditions - the node they are attached to will only execute if all decorators return true
class BTDecorator
{
public:
	BTDecorator() = delete;
	BTDecorator(BehaviourTree* const bt, std::string name = "Unnamed") : _behaviourTree(bt), _name(name) {}

	virtual bool evaluate() { return true; }

protected:
	BehaviourTree* const getBehaviourTree() { return _behaviourTree; }

private:
	std::string _name;
	BehaviourTree* _behaviourTree;
};

class BTNode : public std::enable_shared_from_this<BTNode>
{
public:
	BTNode(BehaviourTree* const bt, std::string name = "Unnamed") : _behaviourTree(bt), _name(name) 
	{
		_blackboard = _behaviourTree->getBlackboard();
	}

	// This method is called every time a node is selected to start
	virtual void run() 
	{
		_behaviourTree->setCurrentNode(this);

		// Return unsuccessfully if decorators don't allow running
		if (!evaluateDecorators())
		{
			onFinished(false);
		}
	}

	// Update runs on the active node until it finishes
	virtual void update(const double&) {}

	// Called when a child node has finished (only works with nodes that allow children)
	virtual void onChildFinished(bool success) {}

	// This method has to be called to prevent the behaviour tree getting stuck in one task
	void onFinished(bool success)
	{
		std::cout << "Node " << _name << " finished " << (success ? "successfully" : "unsuccessfully") << std::endl;

		if (getParentNode() != nullptr)
		{
			getParentNode()->onChildFinished(success);
		}
	}

	// Returns root node of the tree
	class RootNode* const getRootNode()
	{
		// If we have a parent node
		if (getParentNode() != nullptr)
		{
			// If the parent node does not have a parent
			if (getParentNode()->getParentNode() == nullptr)
			{
				// The parent is the root node
				return (RootNode*)getParentNode();
			}

			// Recurse
			return getRootNode();
		}

		// If we don't have a parent node, we are the root node
		return (RootNode*)this;
	}

	// Returns parent node
	BTNode* const getParentNode() { return _parent; }

	// Sets parent node
	virtual void setParentNode(BTNode* const parent) { _parent = parent; }

	// Returns blackboard
	const std::shared_ptr<Blackboard> getBlackboard() { return _blackboard; }

	// Returns name
	const std::string& getName() { return _name; }

	// Add decorators to the node
	void addDecorator(std::shared_ptr<BTDecorator> decorator) { _decorators.push_back(decorator); }
	void addDecorators(std::vector<std::shared_ptr<BTDecorator>> decorators)
	{
		_decorators.insert(_decorators.end(), std::begin(decorators), std::end(decorators)); 
	}

private:
	BTNode* _parent;
	std::shared_ptr<Blackboard> _blackboard;
	BehaviourTree* _behaviourTree;

protected:
	bool evaluateDecorators()
	{
		for (auto decorator : _decorators)
		{
			if (!decorator->evaluate())
			{
				return false;
			}
		}

		return true;
	}

	std::string _name;
	std::vector<std::shared_ptr<BTDecorator>> _decorators;
};

class RootNode : public BTNode
{
public:
	RootNode() = delete;
	RootNode(BehaviourTree* const bt, std::shared_ptr<BTNode> child = nullptr)
		: BTNode(bt, "Root"),  _child(child)
	{

	}

	void setChild(std::shared_ptr<BTNode> child)
	{
		_child = child;
		_child->setParentNode(this);
	}

	virtual void run()
	{
		BTNode::run();

		if (_child != nullptr)
		{
			_child->run();
		}
	}

	virtual void onChildFinished(bool success)
	{
		// Root node always restarts once the child is finished
		run();
	}

private:
	std::shared_ptr<BTNode> _child;
};

// Selector Nodes execute their children from left to right. They stop executing when one of their children succeeds. 
// If a Selector's child succeeds, the Selector succeeds. If all the Selector's children fail, the Selector fails.
class BTSelector : public BTNode
{
private:
	std::vector<std::shared_ptr<BTNode>> _children;
	int _currentId = 0;

public:
	BTSelector() = delete;
	BTSelector(BehaviourTree* const bt, std::string name, std::vector<std::shared_ptr<BTNode>> children)
		: BTNode(bt, name), _children(children)
	{
		for (auto& child : children)
		{
			child->setParentNode(this);
		}
	}

	virtual void run() override
	{
		BTNode::run();

		// If there are no children, return without success
		if (_children.size() == 0)
		{
			onFinished(false);
			return;
		}

		// Wrap current child ID within limit and return true when all children finished successfully
		if (_currentId >= _children.size())
		{
			_currentId = 0;
			return;
		}

		// Run current child node & increment ID counter
		_children[_currentId++]->run();
	}

	virtual void onChildFinished(bool success)
	{
		// When child returns with success, reset the counter and return success
		if (success)
		{
			_currentId = 0;
			onFinished(true);
		}
		else
		{
			// If all children failed, we fail as well; otherwise carry on
			if (_currentId == _children.size())
			{
				onFinished(false);
			} 
			else
			{
				run();
			}
		}
	}
};

// Sequence nodes execute their children from left to right. They stop executing when one of their children fails. 
// If a child fails, then the Sequence fails. If all the Sequence's children succeed, then the Sequence succeeds.
class BTSequence : public BTNode
{
private:
	std::vector<std::shared_ptr<BTNode>> _children;
	int _currentId = 0;

public:
	BTSequence() = delete;
	BTSequence(BehaviourTree* const bt, std::string name, std::vector<std::shared_ptr<BTNode>> children)
		: BTNode(bt, name), _children(children)
	{
		for (auto& child : children)
		{
			child->setParentNode(this);
		}
	}

	virtual void run() override
	{
		BTNode::run();

		// If there are no children, return with success
		if (_children.size() == 0)
		{
			onFinished(true);
			return;
		}

		// Wrap current child ID within limit and return true when all children finished successfully
		if (_currentId >= _children.size())
		{
			_currentId = 0;
			onFinished(true);
			return;
		}

		// Run current child node & increment ID counter
		_children[_currentId++]->run();
	}

	virtual void onChildFinished(bool success)
	{
		// When child returns with success, continue; otherwise return without success
		if (success)
		{
			run();
		}
		else
		{
			// If unsuccessful, reset the child counter and return unsuccessfully
			_currentId = 0;
			onFinished(false);
		}
	}
};