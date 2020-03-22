#pragma once
#include "nlohmann/json.hpp"

using nlohmann::json;

namespace defs
{
	struct Plane
	{
		Plane(){}

		float maxSpeed;
		float acceleration;

		float rotationSpeed;
		float accelerationRotationMultiplier;

		float angularDamping;
		float linearDamping;
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
}