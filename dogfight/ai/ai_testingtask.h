#pragma once
#include "ai/task.h"
#include "system_physics.h"
#include "Box2D/Dynamics/b2WorldCallbacks.h"
#include <string.h>

class BTT_MoveTowardsEntity : public BTTask
{
public:
	BTT_MoveTowardsEntity(BehaviourTree* const bt, std::string name)
		: BTTask(bt, name)
	{
		_timer = 0.5f;
	}

	virtual void run(Entity*) override;
	virtual void update(const double&, Entity*) override;

	// Minimum angle required for the ship to turn
	float MinTurnAngle = 10.0f;

	// Minimum distance at which the ship will accelerate
	float MinThrottleDistance = 50.0f;

private:
	void swap() 
	{
		_timeToSwap = _timer;
		_isAccelerating = !_isAccelerating;
	}

	class Entity* _targetEntity;
	class Enemy* _myPawn;
	bool _isAccelerating;
	float _timer;
	float _timeToSwap;
};

class BTD_IsTargetInFront : public BTDecorator
{
public:
	BTD_IsTargetInFront(BehaviourTree* const bt, std::string name)
		: BTDecorator(bt, name) {}

	virtual bool evaluate(Entity*) override;

	float RayLength = 35;

protected:
	class Entity* _targetEntity;
	class Enemy* _myPawn;
};

class BTD_Cooldown : public BTDecorator
{
public:
	BTD_Cooldown(BehaviourTree* const bt, std::string name, float time)
		: BTDecorator(bt, name), _time(time), _currentTime(0) {}

	virtual bool evaluate(Entity*) override
	{
		if (_currentTime <= 0.0f)
		{
			_currentTime = _time;
			return true;
		}

		return false;
	}

	virtual void update(double& dt, Entity*) override
	{
		if (_currentTime >= 0.0f)
		{
			_currentTime -= dt;
		}
	}

protected:
	float _time;
	float _currentTime;
};

class BTT_FireAtEntity : public BTTask
{
public:
	BTT_FireAtEntity(BehaviourTree* const bt, std::string name, float time)
		: BTTask(bt, name), _time(time)
	{

	}

	virtual void run(Entity*) override;
	virtual void update(const double&, Entity*) override;

private:
	class Entity* _targetEntity;
	class Ship* _myPawn;

	float _time;
	float _currentTime;
};

class TestingTask : public BTTask
{
public:
	TestingTask(BehaviourTree* const bt, std::string name, float time)
		: BTTask(bt, name), _time(time)
	{

	}

	virtual void run(Entity*) override;
	virtual void update(const double&, Entity*) override;

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

	virtual void run(Entity* e) override
	{
		BTTask::run(e);

		onFinished(false);
	};
};

class RaycastCallback : public b2RayCastCallback
{
public:
	std::vector<b2Body*> ignoredBodies;
	b2Body* foundBody;

	float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction) override
	{
		foundBody = fixture->GetBody();
		if (std::find(ignoredBodies.begin(), ignoredBodies.end(), foundBody) != ignoredBodies.end())
		{
			foundBody = nullptr;
			return 1;
		}

		return fraction;
	}
};