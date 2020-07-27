#include "enemy.h"
#include <engine.h>
#include "system_physics.h"

#include "../ai/ai_basicbehaviourtree.h"

Enemy::Enemy(Scene* const s)
	: Ship(s)
{
// 	sf::Vector2f size = { 30.0f, 30.0f };
// 
// 	b2FixtureDef FixtureDef;
// 	FixtureDef.filter.categoryBits = Physics::COLLISION_DYNAMIC;
// 	FixtureDef.filter.maskBits = Physics::MASK_DYNAMIC;
// 
// 	movementComponent = addComponent<PlanePhysicsComponent>(size, FixtureDef);
// 	movementComponent->setDebugDraw(true);
// 
// 	thrusterComponent = addComponent<ShapeComponent>();
// 	thrusterComponent->setShape<sf::RectangleShape>(sf::Vector2f(size.x, size.y / 2.0f));
// 	thrusterComponent->getShape().setFillColor(sf::Color::Yellow);
// 	thrusterComponent->getShape().setOrigin(sf::Vector2f(size.x / 2.0f, size.y / 4.0f));
// 	thrusterComponent->setVisibility(false);
// 	thrusterComponent->setLayer(-1);

	_blackboard = std::make_shared<Blackboard>();
	// Correct
	_blackboard->setEntity("MyPawn", this);
	_blackboard->setEntity("TargetEntity", s->ents.find("Player")[0].get());

	_behaviourTree = s->makeEntity<BasicBehaviourTree>(_blackboard);
	_behaviourTree->startBehaviourTree(this);
}

void Enemy::update(double dt)
{
	Ship::update(dt);
	thrusterComponent->setVisibility(movementComponent->isAccelerating());
	Entity::update(dt);
}
// 
// void Enemy::Accelerate(float Value)
// {
// 	if (movementComponent)
// 	{
// 		movementComponent->accelerate(Value);
// 	}
// }
// 
// void Enemy::Turn(float Value)
// {
// 	movementComponent->turn(Value);
// }
// 
// void Enemy::Fire()
// {
// // 	if (fireCooldown <= 0.0f)
// // 	{
// // 		auto projectile = scene->makeEntity<Projectile>(this);
// // 		projectile->fire(movementComponent->getAngle());
// // 		fireCooldown = 1.0f / fireRate;
// // 	}
// }
