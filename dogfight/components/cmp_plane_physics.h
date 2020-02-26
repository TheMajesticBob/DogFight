#pragma once
#include "cmp_physics.h"
#include "cmp_text.h"

class PlanePhysicsComponent : public PhysicsComponent
{
	protected:
  		b2Vec2 _size;
  		sf::Vector2f _maxVelocity;

		bool _isAccelerating;
		float _maxSpeed;
		float _acceleration;
		float _rotationSpeed;
		float _accelerationRotationMultiplier;


		b2Vec2 forceApplied;
		sf::Text _debugText;
		std::shared_ptr<sf::Font> _font;

	public:
  		void update(double dt) override;
		void render() override;

		// Accelerates the plane with full thrust
		void accelerate();

		// Turns the plane left or right based on the value <-1; 1>
		void turn(float value);

		PlanePhysicsComponent() = delete;
  		explicit PlanePhysicsComponent(Entity* p, const sf::Vector2f& size);
};