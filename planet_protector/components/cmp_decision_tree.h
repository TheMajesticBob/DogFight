//cmp_decision_tree.h
#pragma once

#include <ecm.h>
#include <memory>
#include <vector>

class DecisionTreeNode
{
	public:
		virtual ~DecisionTreeNode() = default;
		virtual void makeDecision(Entity *owner) = 0;
};

class Decision : public DecisionTreeNode
{
	protected:
		std::shared_ptr<DecisionTreeNode> _trueNode;
		std::shared_ptr<DecisionTreeNode> _falseNode;
		virtual std::shared_ptr<DecisionTreeNode> getBranch(Entity *owner) = 0;
	public:
		Decision(std::shared_ptr<DecisionTreeNode> trueNode, std::shared_ptr<DecisionTreeNode> falseNode) : _trueNode(trueNode), _falseNode(falseNode) { }

		void makeDecision(Entity *owner)
		{
			getBranch(owner)->makeDecision(owner);
		}
};

class MultiDecision : public DecisionTreeNode
{
	protected:
		std::vector<std::shared_ptr<DecisionTreeNode>> _childNodes;
		virtual std::shared_ptr<DecisionTreeNode> getBranch(Entity *owner) = 0;
	public:
		MultiDecision(const std::vector<std::shared_ptr<DecisionTreeNode>> &childNodes) : _childNodes(childNodes) { }

		void makeDecision(Entity *owner)
		{
			getBranch(owner)->makeDecision(owner);
		}
};

class RandomDecision : public Decision
{
	protected:
		std::shared_ptr<DecisionTreeNode> getBranch(Entity *owner) final;
	public:
		RandomDecision(std::shared_ptr<DecisionTreeNode> trueNode, std::shared_ptr<DecisionTreeNode> falseNode) : Decision(trueNode, falseNode) { }
};

class RandomMultiDecision : public MultiDecision
{
	protected:
		std::shared_ptr<DecisionTreeNode> getBranch(Entity *owner) override final;
	public:
		RandomMultiDecision(const std::vector<std::shared_ptr<DecisionTreeNode>> &childNodes) : MultiDecision(childNodes) { }
};

class DecisionTreeComponent : public Component
{
	private:
		std::shared_ptr<DecisionTreeNode> _decisionTree;
	public:
		void update(double) override;
		void render() override { }
		explicit DecisionTreeComponent(Entity *p, std::shared_ptr<DecisionTreeNode> decisionTree);
		DecisionTreeComponent() = delete;
};