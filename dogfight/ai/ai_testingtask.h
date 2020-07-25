#pragma once
#include "ai/task.h"
#include <string.h>

class BTT_MoveTowardsEntity : public BTTask
{
public:
	BTT_MoveTowardsEntity(BehaviourTree* const bt, std::string name)
		: BTTask(bt, name)
	{
		_timer = 0.5f;
	}

	virtual void run() override;
	virtual void update(const double&) override;

private:
	void swap() 
	{
		_timeToSwap = _timer;
		_isAccelerating = !_isAccelerating;
	}

	std::shared_ptr<Entity> _targetEntity;
	class Enemy* _myPawn;
	bool _isAccelerating;
	float _timer;
	float _timeToSwap;
};

class TestingDecorator : public BTDecorator
{
public:
	TestingDecorator(BehaviourTree* const bt, std::string name)
		: BTDecorator(bt, name) {}

	virtual bool evaluate()
	{
		std::cout << "Decorator fails" << std::endl;
		return false;
	}
};

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