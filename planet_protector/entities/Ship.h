#pragma once
#include <ecm.h>
#include "../components/cmp_weapon.h"
#include "../components/cmp_plane_physics.h"
#include "../components/cmp_sprite.h"
#include "GL/glew.h"
#include <SFML/Graphics.hpp>
#include "SFML/OpenGL.hpp"
#include <delegates.h>

DEFINE_DELEGATE(FShipDestroyed, std::shared_ptr<class Entity>);
DEFINE_DELEGATE(FOnDestroyed);

class Ship : public Pawn
{
	public:

		Ship() = delete;
		Ship(Scene* const s, std::string shipDefinition);

		void update(double) override;
		void render() override;

		void OnHit(float damage) override;
		virtual void OnDestroyed();

		sf::Color GetColor() { return _color; }

		void SetColor(sf::Color color)
		{
			_color = color;
			sf::ConvexShape* shape = (sf::ConvexShape*)&shapeComponent->getShape();
			if (shape)
			{
				shape->setOutlineColor(color);
			}
			for (std::shared_ptr<WeaponComponent> w : weaponComponents)
			{
				w->getShape().setOutlineColor(color);
			}
		}

		std::shared_ptr<PlanePhysicsComponent> GetMovementComponent() { return movementComponent; }
		std::shared_ptr<class HealthComponent> GetHealthComponent() { return healthComponent; }

		b2Fixture* const GetFixture() { return movementComponent->getFixture(); }

		void SetGodMode(bool godActive) { _godMode = godActive; }

		void Accelerate(float Value);
		void Turn(float Value);

		void StartFiring() { _isFiring = true; }
		void StopFiring() { _isFiring = false; }

		FShipDestroyed onShipDestroyed;

	protected:
		void OnShieldHit(float damage);
		void OnHullHit(float damage);
		void Fire();

		void OnWeaponFired();

		std::string GetHealth();

		std::shared_ptr<defs::Ship> _shipDefinition;

		std::shared_ptr<class ThrusterParticleSystem> thrusterPS;
		std::shared_ptr<PlanePhysicsComponent> movementComponent;
		std::shared_ptr<class HealthComponent> healthComponent;
		std::vector<std::shared_ptr<class WeaponComponent>> weaponComponents;
		std::shared_ptr<ShapeComponent> shapeComponent;
		std::shared_ptr<ShapeComponent> shieldComponent;

		bool _godMode = false;

		bool _isFiring = false;

		float fireRate = 3.0f;
		float fireCooldown = 0.0f;

		GLfloat drawVerts[8];
		sf::Color _color;


		float _shieldVisibility = 0.0f;
};