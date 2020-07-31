#pragma once
#include <ecm.h>
#include "../components/cmp_weapon.h"
#include "../components/cmp_plane_physics.h"
#include "../components/cmp_sprite.h"
#include "GL/glew.h"
#include <SFML/Graphics.hpp>
#include "SFML/OpenGL.hpp"

class Ship : public Pawn
{
	public:
		enum Team
		{
			T_PLAYER,
			T_ENEMY,
			T_TEAM3,
			T_TEAM4
		};

		Ship() = delete;
		Ship(Scene* const s, std::string shipDefinition);

		void update(double) override;
		void render() override;

		void OnHit(float damage) override;
		virtual void OnDestroyed();

		void SetTeam(Team newTeam) { team = newTeam; }
		const Team& GetTeam() { return team; }

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
		void Fire();

	protected:
		void OnShieldHit(float damage);
		void OnHullHit(float damage);

		std::string GetHealth();

		std::shared_ptr<defs::Ship> _shipDefinition;

		std::shared_ptr<class ThrusterParticleSystem> thrusterPS;
		std::shared_ptr<PlanePhysicsComponent> movementComponent;
		std::shared_ptr<class HealthComponent> healthComponent;
		std::vector<std::shared_ptr<class WeaponComponent>> weaponComponents;
		std::shared_ptr<ShapeComponent> shapeComponent;
		std::shared_ptr<ShapeComponent> thrusterComponent;

		Team team;

		bool _godMode = false;

		float fireRate = 3.0f;
		float fireCooldown = 0.0f;

		GLfloat drawVerts[8];
		sf::Color _color;
};