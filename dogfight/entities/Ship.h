#pragma once
#include <ecm.h>
#include "../components/cmp_plane_physics.h"
#include "../components/cmp_sprite.h"
#include <SFML/Graphics.hpp>

class Ship : public Pawn
{
	public:
		Ship() = delete;
		Ship(Scene* const s);

		void update(double) override;
		std::shared_ptr<PlanePhysicsComponent> GetMovementComponent() { return movementComponent; }
		b2Fixture* const GetFixture() { return movementComponent->getFixture(); }

		void Accelerate(float Value);
		void Turn(float Value);
		void Fire();

	protected:
		std::shared_ptr<PlanePhysicsComponent> movementComponent;
		std::shared_ptr<ShapeComponent> shapeComponent;
		std::shared_ptr<ShapeComponent> thrusterComponent;

		float fireRate = 3.0f;
		float fireCooldown = 0.0f;
};

class Player : public Ship
{
public:
	Player() = delete;
	Player(Scene* const s);

	void update(double) override;
};

class TriangleShape : sf::CircleShape
{
	public :

    explicit TriangleShape(const sf::Vector2f radius = {0.0f, 0.0f}) :
    m_radius(radius)
    {
        update();
		setPointCount(3);
    }

	private:
		sf::Vector2f m_radius;

};