#pragma once
#include "ai/task.h"
#include <string.h>

class TestingTask : public BTTask
{
public:
	TestingTask(BehaviourTree* const bt, std::string name, float time)
		: BTTask(bt, name), _time(time)
	{

	}

	virtual void run() override;
	virtual void update(const double&) override;

private:
	float _time;
	float _currentTime;
};

class FailTask : public BTTask
{
public:
	FailTask(BehaviourTree* const bt, std::string name)
		: BTTask(bt, name)
	{

	}

	virtual void run() override 
	{
		BTTask::run();

		onFinished(false);
	};
};