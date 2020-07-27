#pragma once
#include "nlohmann/json.hpp"

using nlohmann::json;

namespace defs
{
	struct Plane
	{
		Plane() {}

		float maxSpeed;
		float acceleration;

		float rotationSpeed;
		float accelerationRotationMultiplier;

		float angularDamping;
		float linearDamping;
	};

	struct Projectile
	{
		Projectile() {}

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
		j.at("PlaneControl").at("MaxSpeed").get_to(p.maxSpeed);
		j.at("PlaneControl").at("RotationSpeed").get_to(p.rotationSpeed);
		j.at("PlaneControl").at("Acceleration").get_to(p.acceleration);
		j.at("PlaneControl").at("AccelerationRotationMultiplier").get_to(p.accelerationRotationMultiplier);
		j.at("Physics").at("AngularDamping").get_to(p.angularDamping);
		j.at("Physics").at("LinearDamping").get_to(p.linearDamping);
	}

	inline void from_json(const json &j, Projectile &p)
	{
		j.at("Projectile").at("InitialSpeed").get_to(p.initialSpeed);
		j.at("Physics").at("Mass").get_to(p.mass);
		j.at("Physics").at("LinearDamping").get_to(p.linearDamping);
		j.at("Physics").at("GravityAffects").get_to(p.gravityAffects);
	}
}