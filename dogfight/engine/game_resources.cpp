#include "game_resources.h"
#include <fstream>

using json = nlohmann::json;

namespace Resources
{
	template<>
	std::shared_ptr<defs::Plane> load(const std::string &name)
	{
    	auto f = std::make_shared<defs::Plane>();
		std::ifstream file("res/data/planes/" + name + ".json");
		if(!file.is_open())
		{
      		throw("not found: " + name);
    	};
		json j;
		file >> j;
		*f = j.get<defs::Plane>();
    	return f;
	};

	template<>
	std::shared_ptr<defs::Weapon> load(const std::string& name)
	{
		auto f = std::make_shared<defs::Weapon>();
		std::ifstream file("res/data/weapons/" + name + ".json");
		if (!file.is_open())
		{
			throw("not found: " + name);
		};
		json j;
		file >> j;
		*f = j.get<defs::Weapon>();
		return f;
	};

	template<>
	std::shared_ptr<defs::Projectile> load(const std::string &name)
	{
		auto f = std::make_shared<defs::Projectile>();
		std::ifstream file("res/data/projectiles/" + name + ".json");
		if (!file.is_open())
		{
			throw("not found: " + name);
		};
		json j;
		file >> j;
		*f = j.get<defs::Projectile>();
		return f;
	};

	template<>
	std::shared_ptr<defs::Controls> load(const std::string& name)
	{
		auto f = std::make_shared<defs::Controls>();
		std::ifstream file("res/data/settings" + name + ".json");
		if (!file.is_open())
		{
			throw("not found: " + name);
		};
		json j;
		file >> j;
		*f = j.get<defs::Controls>();
		return f;
	};

}