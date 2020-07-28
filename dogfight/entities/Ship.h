#pragma once
#include <ecm.h>
#include "../components/cmp_plane_physics.h"
#include "../components/cmp_sprite.h"
#include <SFML/Graphics.hpp>
#include "SFML/OpenGL.hpp"

class Ship : public Pawn
{
	public:
		Ship() = delete;
		Ship(Scene* const s, std::string shipDefinition);

		void update(double) override;
		void render() override;

		void OnHit(float damage) override;
		virtual void OnDestroyed();

		std::shared_ptr<PlanePhysicsComponent> GetMovementComponent() { return movementComponent; }
		std::shared_ptr<class HealthComponent> GetHealthComponent() { return healthComponent; }

		b2Fixture* const GetFixture() { return movementComponent->getFixture(); }

		void Accelerate(float Value);
		void Turn(float Value);
		void Fire();

	protected:
		std::shared_ptr<defs::Plane> _shipDefinition;

		std::shared_ptr<PlanePhysicsComponent> movementComponent;
		std::shared_ptr<class HealthComponent> healthComponent;
		std::vector<std::shared_ptr<class WeaponComponent>> weaponComponents;
		std::shared_ptr<ShapeComponent> shapeComponent;
		std::shared_ptr<ShapeComponent> thrusterComponent;

		float fireRate = 3.0f;
		float fireCooldown = 0.0f;

		GLfloat drawVerts[8];
		sf::Color color;
};