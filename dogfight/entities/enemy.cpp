#include "enemy.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>

using namespace sf;

Enemy::Enemy(Scene* const s) : Entity(s)
{
	Vector2f size = { 30.0f, 30.0f };

	movementComponent = addComponent<PlanePhysicsComponent>(size);
	movementComponent->setDebugDraw(true);

	shapeComponent = addComponent<ShapeComponent>();
	shapeComponent->setShape<CircleShape>(size.x / 2.0f, 3);
	shapeComponent->getShape().setFillColor(Color::Red);
	shapeComponent->getShape().setOrigin({15.0f, 10.0f});
	shapeComponent->getShape().setRotation(45.0f);

}

void Enemy::update(double dt)
{
	Entity::update(dt);

	movementComponent->accelerate();
}
