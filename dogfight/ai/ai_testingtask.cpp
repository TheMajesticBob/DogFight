#include "ai_testingtask.h"
#include "ai_basicbehaviourtree.h"
#include "../entities/enemy.h"
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
		_myPawn = static_cast<Enemy*>(getBlackboard()->getEntity("MyPawn"));
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

void BTT_MoveTowardsEntity::update(const double& dt, Entity* e)
{
	_timeToSwap -= dt;
	if (_timeToSwap <= 0.0f)
	{
		swap();
	}

	if (_myPawn)
	{
		auto movementComponent = _myPawn->GetMovementComponent();
		if (movementComponent)
		{
			Blackboard* blackboard = getBlackboard().get();
			if (blackboard)
			{
				_targetEntity = blackboard->getEntity("TargetEntity");
			}

			sf::Vector2f direction;
			float distance;

			if (_targetEntity)
			{
				direction = sf::normalize(_targetEntity->getPosition() - _myPawn->getPosition());
				direction.y = -direction.y;
				distance = sf::length(_targetEntity->getPosition() - _myPawn->getPosition());
			}

			sf::Vector2f forward = _myPawn->GetMovementComponent()->getForwardVector();
			float _dot = sf::dot(direction, forward);
			float angle = sf::rad2deg(atan2(direction.x * forward.y - forward.x * direction.y, direction.x * forward.x + direction.y * forward.y)); // +90.0f;

			if (abs(angle) > MinTurnAngle) // && _dot > -0.9f)
			{
				_myPawn->Turn(angle);
				_myPawn->Accelerate(0.1f);
			}
			else
			{
				if (distance > MinThrottleDistance)
				{
					_myPawn->Accelerate(distance / 10000.0f);
				}
				else
				{
					onFinished(true);
				}
			}
		}
	}

// 	if (_myPawn && _isAccelerating)
// 	{
// 		_myPawn->Accelerate(0.5f);
// 	}
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
			_myPawn = static_cast<Enemy*>(blackboard->getEntity("MyPawn"));
			_targetEntity = blackboard->getEntity("TargetEntity");

			if (_myPawn && _targetEntity)
			{
				sf::Vector2f forward = _myPawn->GetMovementComponent()->getForwardVector();
				forward.y = -forward.y;

				sf::Vector2f myPosition = _myPawn->getPosition() + forward * 50.f;
				sf::Vector2f targetPosition = _targetEntity->getPosition();

				// Draw debug line
// 				glColor3f(1, 1, 1);
// 				glBegin(GL_LINES);
// 				glVertex2f(myPosition.x, myPosition.y);
// 				glVertex2f(targetPosition.x, targetPosition.y);
// 				glEnd();
 
				// Use custom Raycast Callback
				RaycastCallback* callback = new RaycastCallback();
				callback->ignoredBodies.push_back(_myPawn->GetMovementComponent()->getFixture()->GetBody());
				Physics::GetWorld()->RayCast(callback, Physics::sv2_to_bv2(myPosition), Physics::sv2_to_bv2(targetPosition));

				if (callback->foundBody)
				{
					Player* player = static_cast<Player*>(callback->foundBody->GetUserData());
					if (player != nullptr)
					{
						return true;
					}
				}
// 				// Setup raycast input
// 				b2RayCastInput input;
// 				input.p1 = Physics::sv2_to_bv2(myPosition);
// 				input.p2 = Physics::sv2_to_bv2(targetPosition);
// 				input.maxFraction = 1;
// 
// 				// Check every fixture of every body to find closest
// 				float closestFraction = 1; // Start with end of line as p2
// 				b2Vec2 intersectionNormal(0, 0);
// 				for (b2Body* b = Physics::GetWorld()->GetBodyList(); b; b = b->GetNext())
// 				{
// 					for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
// 					{
// 
// 						if (output.fraction < closestFraction)
// 						{
// 							closestFraction = output.fraction;
// 							intersectionNormal = output.normal;
// 						}
// 					}
// 				}
			}
		}
	}

	return false;
}

void BTT_FireAtEntity::run(Entity* e)
{
	BTTask::run(e);

	_currentTime = _time;

	Blackboard* blackboard = getBlackboard().get();
	if (blackboard)
	{
		_targetEntity = blackboard->getEntity("TargetEntity");
		_myPawn = static_cast<Ship*>(e);
	}
}

void BTT_FireAtEntity::update(const double& dt, Entity* e)
{
	if (!_myPawn || !_targetEntity)
	{
		onFinished(false);
		return;
	}

	if (_currentTime <= 0.0f)
	{
		onFinished(true);
		return;
	}

	sf::Vector2f direction;
	float distance;

	direction = sf::normalize(_targetEntity->getPosition() - _myPawn->getPosition());
	direction.y = -direction.y;
	distance = sf::length(_targetEntity->getPosition() - _myPawn->getPosition());

	sf::Vector2f forward = _myPawn->GetMovementComponent()->getForwardVector();
	float _dot = sf::dot(direction, forward);
	float angle = sf::rad2deg(atan2(direction.x * forward.y - forward.x * direction.y, direction.x * forward.x + direction.y * forward.y)); // +90.0f;

	if (abs(angle) > 1.0f) // && _dot > -0.9f)
	{
		_myPawn->Turn(angle);
	} 
	else
	{
		_myPawn->Fire();
	}

	_currentTime -= dt;
}
