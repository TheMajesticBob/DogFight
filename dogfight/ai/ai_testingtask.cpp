#include "ai_testingtask.h"
#include "ai_basicbehaviourtree.h"
#include "../entities/ship.h"
#include <SFML/OpenGL.hpp>
#include "Box2D/Dynamics/b2World.h"

void TestingTask::run(Entity* e)
{
	BTTask::run(e);

	_currentTime = _time;
}

void TestingTask::update(const double& dt, Entity* e)
{
	_currentTime -= dt;

	if (_currentTime <= 0.0f)
	{
		onFinished(true);
	}
}

void BTT_MoveTowardsEntity::run(Entity* e)
{
	BTTask::run(e);

	if (getBlackboard())
	{
		_myPawn = static_cast<Ship*>(getBlackboard()->getEntity("MyPawn"));
	}
}

void BTT_MoveTowardsEntity::update(const double& dt, Entity* e)
{
	if (_myPawn)
	{
		auto movementComponent = _myPawn->GetMovementComponent();
		if (movementComponent)
		{
			// Get current target entity
			Blackboard* blackboard = getBlackboard().get();
			if (blackboard)
			{
				_targetEntity = blackboard->getEntity("TargetEntity");
			}

			// Return unsuccessfully when there is no target
			if (!_targetEntity)
			{
				onFinished(false);
			}

			// Calculate direction and distance to target
			sf::Vector2f forward = _myPawn->GetMovementComponent()->getForwardVector();
			sf::Vector2f direction = sf::normalize(_targetEntity->getPosition() - _myPawn->getPosition());
			direction.y = -direction.y; // For some reason Y is inverted...
			float distance = sf::length(_targetEntity->getPosition() - _myPawn->getPosition());

			// Calculate shortest angle towards the target
			float angle = sf::rad2deg(atan2(direction.x * forward.y - forward.x * direction.y, direction.x * forward.x + direction.y * forward.y)); // +90.0f;

			// Rotate towards the target and accelerate
			_myPawn->Turn(angle);

			if (abs(angle) > MinTurnAngle)
			{
				_myPawn->Accelerate(MinTurnAngle/abs(angle));
			}
			else
			{
				// Go full throttle if too far
				if (distance > MinThrottleDistance)
				{
					_myPawn->Accelerate(1.0f);
				}
				else
				{
					// Finished successfully when within set range
					onFinished(true);
				}
			}
		}
	}
}

bool BTD_IsTargetInFront::evaluate(Entity* e)
{
	// Always check validity first
	BehaviourTree* behaviourTree = getBehaviourTree();
	if (behaviourTree)
	{
		// Get the blackboard
		Blackboard* blackboard = behaviourTree->getBlackboard().get(); // static_cast<EnemyBlackboard*>(getBehaviourTree()->getBlackboard().get());
		if (blackboard)
		{
			// Get the entities of interest
			_myPawn = static_cast<Ship*>(blackboard->getEntity("MyPawn"));
			_targetEntity = blackboard->getEntity("TargetEntity");

			if (_myPawn && _targetEntity)
			{
				sf::Vector2f forward = _myPawn->GetMovementComponent()->getForwardVector();
				forward.y = -forward.y;

				sf::Vector2f myPosition = _myPawn->getPosition() + forward * 50.f;
				sf::Vector2f targetPosition = _targetEntity->getPosition();
 
				// Use custom Raycast Callback, ignore caller's body
				RaycastCallback* callback = new RaycastCallback();
				callback->ignoredBodies.push_back(_myPawn->GetMovementComponent()->getFixture()->GetBody());

				Physics::GetWorld()->RayCast(callback, Physics::sv2_to_bv2(myPosition), Physics::sv2_to_bv2(targetPosition));

				// If we found a body, check whether it's our target
				if (callback->foundBody)
				{
					Entity* target = static_cast<Entity*>(callback->foundBody->GetUserData());
					if (target != _targetEntity)
					{
						return true;
					}
				}
			}
		}
	}

	// If we got this far, target is not in front of us
	return false;
}

void BTT_FireAtEntity::run(Entity* e)
{
	BTTask::run(e);

	// Setup the timer
	_currentTime = _time;

	// Get entities of interest
	Blackboard* blackboard = getBlackboard().get();
	if (blackboard)
	{
		_targetEntity = blackboard->getEntity("TargetEntity");
		_myPawn = static_cast<Ship*>(e);
	}
}

void BTT_FireAtEntity::update(const double& dt, Entity* e)
{
	// Finish unsuccessfully when one of the entities does not exist
	if (!_myPawn || !_targetEntity)
	{
		onFinished(false);
		return;
	}

	// Finish successfully when the timer has finished
	if (_currentTime <= 0.0f)
	{
		onFinished(true);
		return;
	}

	// Rotate towards the player and shoot
	sf::Vector2f forward = _myPawn->GetMovementComponent()->getForwardVector();
	sf::Vector2f direction = sf::normalize(_targetEntity->getPosition() - _myPawn->getPosition());
	direction.y = -direction.y;
	float angle = sf::rad2deg(atan2(direction.x * forward.y - forward.x * direction.y, direction.x * forward.x + direction.y * forward.y)); // +90.0f;

	if (abs(angle) > 1.0f)
	{
		_myPawn->Turn(angle);
	} 
	else
	{
		_myPawn->Fire();
	}

	_currentTime -= dt;
}
