#pragma once
#include "cmp_physics.h"
#include "cmp_text.h"
#include "../definitions.h"
#include <SFML/Graphics.hpp>

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

		std::shared_ptr<defs::Plane> _planeDefinition;

		b2Vec2 forceApplied;
		sf::Text _debugText;
		std::shared_ptr<sf::Font> _font;

		bool _debugDraw;
		sf::ConvexShape _debugShape;

	public:
  		void update(double dt) override;
		void render() override;

		// Accelerates the plane with full thrust
		void accelerate(float value = 1.0f);

		// Turns the plane left or right based on the value <-1; 1>
		void turn(float value);

		void setDebugDraw(bool draw) { _debugDraw = draw; }
		bool getDebugDraw() { return _debugDraw; }

		bool isAccelerating() { return _isAccelerating; }

		sf::Vector2f getForwardVector();

		PlanePhysicsComponent() = delete;
  		explicit PlanePhysicsComponent(Entity* p, const sf::Vector2f& size, b2FixtureDef& fixtureDef = b2FixtureDef());
};