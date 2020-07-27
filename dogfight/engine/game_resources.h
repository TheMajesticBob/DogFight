#pragma once
#include <memory>
#include <string>
#include "system_resources.h"
#include "../definitions.h"

namespace Resources
{
	template <> // explicit specialization for T = Plane
	std::shared_ptr<defs::Plane> load(const std::string& name);

	template <> // explicit specialization for T = Weapon
	std::shared_ptr<defs::Weapon> load(const std::string& name);

	template <> // explicit specialization for T = Projectile
	std::shared_ptr<defs::Projectile> load(const std::string& name);

	template <> // explicit specialization for T = Controls
	std::shared_ptr<defs::Controls> load(const std::string& name);
}