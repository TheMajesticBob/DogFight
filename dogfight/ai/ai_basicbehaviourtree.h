#pragma once
#include "ai/behaviour_tree.h"
#include "ai_testingtask.h"

class BasicBehaviourTree : public BehaviourTree
{
public:
	BasicBehaviourTree(Scene* const s, std::shared_ptr<Blackboard> b = nullptr)
		: BehaviourTree(s, b)
	{
		std::shared_ptr<TestingDecorator> decorator = makeDecorator<TestingDecorator>("Fail decorator");

		std::shared_ptr<TestingTask> task1 = makeNode<TestingTask>("First task", 2.0f);
		std::shared_ptr<TestingTask> task2 = makeNode<TestingTask>("Second task", 1.0f);
		task2->addDecorator(decorator);
		std::shared_ptr<TestingTask> task3 = makeNode<TestingTask>("Third task", 3.0f);
		std::shared_ptr<TestingTask> task4 = makeNode<TestingTask>("Fourth task", 0.5f);

		std::shared_ptr<TestingTask> task5 = makeNode<TestingTask>("First inner task", 2.0f);
		std::shared_ptr<TestingTask> task6 = makeNode<TestingTask>("Second inner task", 1.0f);
		std::shared_ptr<FailTask> task7 = makeNode<FailTask>("Fail task");

		std::shared_ptr<BTSelector> sequence2 = makeNode<BTSelector>("Inner Selector",
			std::vector<std::shared_ptr<BTNode>>
			{
				std::static_pointer_cast<BTNode>(task7),
				std::static_pointer_cast<BTNode>(task5),
				std::static_pointer_cast<BTNode>(task6)
			});

		std::shared_ptr<BTSequence> sequence = makeNode<BTSequence>("Sequence",
			std::vector<std::shared_ptr<BTNode>>
		{
			std::static_pointer_cast<BTNode>(task1),
				std::static_pointer_cast<BTNode>(task2),
				std::static_pointer_cast<BTNode>(sequence2),
				std::static_pointer_cast<BTNode>(task3),
				std::static_pointer_cast<BTNode>(task4)
		});

		getRootNode()->setChild(sequence);
	}
};