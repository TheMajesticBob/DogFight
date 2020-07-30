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
				return;
			}

			// Calculate direction and distance to target
			sf::Vector2f forward = _myPawn->GetMovementComponent()->getForwardVector();
			sf::Vector2f direction = sf::normalize(_targetEntity->getPosition() - _myPawn->getPosition());
			direction.y = -direction.y; // For some reason Y is inverted...
			float distance = sf::length(_targetEntity->getPosition() - _myPawn->getPosition());

			// Calculate shortest angle towards the target
			float angle = sf::rad2deg(atan2(direction.x * forward.y - forward.x * direction.y, direction.x * forward.x + direction.y * forward.y)); // +90.0f;
			
			// Check if we're close enough
			if (distance < MinThrottleDistance)
			{
				// Finished successfully when within set range
				onFinished(true);
				return;
			}

			// Rotate towards the target and accelerate
			_myPawn->Turn(angle);

			if (abs(angle) > MinTurnAngle)
			{
				_myPawn->Accelerate(MinTurnAngle/abs(angle));
			}
			else
			{
				_myPawn->Accelerate(1.0f);
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

			if (_myPawn && _targetEntity && _targetEntity->isAlive())
			{
				sf::Vector2f forward = _myPawn->GetMovementComponent()->getForwardVector();
				forward.y = -forward.y;

				sf::Vector2f myPosition = _myPawn->getPosition() + forward * 50.f;
				sf::Vector2f targetPosition = _targetEntity->getPosition();
 
				// Use custom Raycast Callback, ignore caller's body
				RaycastCallback* callback = new RaycastCallback();
				callback->ignoredBodies.push_back(_myPawn->GetMovementComponent()->getFixture()->GetBody());

				Physics::GetWorld()->RayCast(callback, Physics::sv2_to_bv2(Physics::invert_height(myPosition)), Physics::sv2_to_bv2(Physics::invert_height(targetPosition)));

				glLineWidth(.1f);
				glColor4f(0, 255, 0, 255);
				glBegin(GL_LINES);
				glVertex2f(myPosition.x, myPosition.y);
				glVertex2f(targetPosition.x, targetPosition.y);

				// If we found a body, check whether it's our target
				if (callback->foundBody)
				{
					glColor4f(255, 0, 0, 255);
					glVertex2f(myPosition.x, myPosition.y);
					glVertex2f(Physics::invert_height(Physics::bv2_to_sv2(callback->foundBody->GetPosition())).x, Physics::invert_height(Physics::bv2_to_sv2(callback->foundBody->GetPosition())).y);

					Entity* target = static_cast<Entity*>(callback->foundBody->GetUserData());
					if (target == _targetEntity)
					{
						glEnd();
						return true;
					}
				}

				glEnd();
			}
		}
	}

	//std::cout << "Target not in front" << std::endl;
	// If we got this far, target is not in front of us
	return false;
}

void BTT_FireAtEntity::run(Entity* e)
{
	//std::cout << "Running fire at entity" << std::endl;
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
