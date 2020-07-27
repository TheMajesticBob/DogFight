#pragma once
#include "nlohmann/json.hpp"

using nlohmann::json;

namespace defs
{
	struct Plane
	{
		Plane() {}

		float health;
		float shield;

		std::vector<std::string> weapons;

		float maxSpeed;
		float acceleration;

		float rotationSpeed;
		float accelerationRotationMultiplier;

		float angularDamping;
		float linearDamping;
	};

	struct Weapon
	{
		Weapon() {}

		std::string projectile;

		float damage;
		float rateOfFire;
		float projectilesPerRound;
	};

	struct Projectile
	{
		Projectile() {}

		sf::Vector2f size;
		float sizeX;
		float sizeY;

		float initialSpeed;
		float linearDamping;

		float mass;

		bool gravityAffects;
	};

	struct Controls
	{
		Controls() {}
			   
		int accelerate;
		int turnLeft;
		int turnRight;

		int shoot;
	};

	inline void from_json(const json &j, Plane &p)
	{
		j.at("Health").at("Hull").get_to(p.health);
		j.at("Health").at("Shield").get_to(p.shield);
		j.at("Weapons").get_to(p.weapons);

		j.at("PlaneControl").at("MaxSpeed").get_to(p.maxSpeed);
		j.at("PlaneControl").at("RotationSpeed").get_to(p.rotationSpeed);
		j.at("PlaneControl").at("Acceleration").get_to(p.acceleration);
		j.at("PlaneControl").at("AccelerationRotationMultiplier").get_to(p.accelerationRotationMultiplier);
		j.at("Physics").at("AngularDamping").get_to(p.angularDamping);
		j.at("Physics").at("LinearDamping").get_to(p.linearDamping);
	}

	inline void from_json(const json &j, Weapon &w)
	{
		j.at("Projectile").get_to(w.projectile);
		j.at("Damage").get_to(w.damage);
		j.at("RateOfFire").get_to(w.rateOfFire);
		j.at("ProjectilesPerRound").get_to(w.projectilesPerRound);
	}

	inline void from_json(const json &j, Projectile &p)
	{
		j.at("Projectile").at("Size").at("X").get_to(p.size.x);
		j.at("Projectile").at("Size").at("Y").get_to(p.size.y);
		j.at("Projectile").at("InitialSpeed").get_to(p.initialSpeed);
		j.at("Physics").at("Mass").get_to(p.mass);
		j.at("Physics").at("LinearDamping").get_to(p.linearDamping);
		j.at("Physics").at("GravityAffects").get_to(p.gravityAffects);
	}

	inline void from_json(const json &j, Controls &c)
	{
		j.at("Movement").at("Accelerate").get_to(c.accelerate);
		j.at("Movement").at("Left").get_to(c.turnLeft);
		j.at("Movement").at("Right").get_to(c.turnRight);
		j.at("Weapons").at("BasicAttack").get_to(c.shoot);
	}
}