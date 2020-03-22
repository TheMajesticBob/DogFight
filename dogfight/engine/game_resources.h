#pragma once
#include <memory>
#include <string>
#include "system_resources.h"
#include "../definitions.h"

namespace Resources
{
	template <> // explicit specialization for T = texture
	std::shared_ptr<defs::Plane> load(const std::string& name);
}