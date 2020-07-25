#include "ai_testingtask.h"
#include "ai_basicbehaviourtree.h"
#include "../entities/enemy.h"

void TestingTask::run()
{
	BTTask::run();

	_currentTime = _time;
	// std::cout << "Task " << _name << " started";
}

void TestingTask::update(const double& dt)
{
	_currentTime -= dt;

	// std::cout << _currentTime << std::endl;

	if (_currentTime <= 0.0f)
	{
		onFinished(true);
	}
}

void BTT_MoveTowardsEntity::run()
{
	BTTask::run();

	if (getBlackboard())
	{
 		EnemyBlackboard* blackboard = static_cast<EnemyBlackboard*>(getBlackboard().get());
		if (blackboard)
		{
			_myPawn = blackboard->myPawn;
		}
// 		if (!blackboard || !blackboard->targetEntity)
// 		{
// 			onFinished(false);	
// 		}

	}
	else 
	{
		//onFinished(false);
	}
}

void BTT_MoveTowardsEntity::update(const double& dt)
{
	_timeToSwap -= dt;
	if (_timeToSwap <= 0.0f)
	{
		swap();
	}

	if (_myPawn)
	{
		EnemyBlackboard* blackboard = static_cast<EnemyBlackboard*>(getBlackboard().get());
		if (blackboard)
		{
			_targetEntity = blackboard->targetEntity;
		}

		sf::Vector2f direction;
		float distance;

		if (_targetEntity)
		{
			direction = sf::normalize(_targetEntity->getPosition() - _myPawn->getPosition());
			distance = sf::length(_targetEntity->getPosition() - _myPawn->getPosition());
		}

		float _dot = sf::dot(direction, _myPawn->GetMovementComponent()->getForwardVector());
		float angle = sf::rad2deg(acos(_dot)); // +90.0f;

		if (abs(angle) > 10.0f)
		{
			_myPawn->Turn(angle / 10.0f);
		}
		else
		{
			if (distance > 10.0f)
			{
				_myPawn->Accelerate(distance / 1000.0f);
			}
		}
	}

// 	if (_myPawn && _isAccelerating)
// 	{
// 		_myPawn->Accelerate(0.5f);
// 	}
}
