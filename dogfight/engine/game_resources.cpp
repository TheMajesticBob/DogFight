#include "game_resources.h"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

namespace Resources
{
	template<typename T>
	const std::string makeName(const std::string &name)
	{
		std::string appendix = "res/data/";
		std::string subDirectory = "";
		std::string fileName = name;

		size_t directory = name.find_last_of("/");

		if (directory != std::string::npos)
		{
			subDirectory = name.substr(0, directory + 1);
			fileName = name.substr(directory + 1);
		}

		if (std::is_base_of_v<T, defs::Ship>)
		{
			appendix += "ships/" + subDirectory + "ship_";
		}
		else if (std::is_base_of_v<T, defs::Weapon>)
		{
			appendix += "weapons/" + subDirectory + "weapon_";
		}
		else if (std::is_base_of_v<T, defs::GameShape>)
		{
			appendix += "shapes/" + subDirectory + "shape_";
		}
		else if (std::is_base_of_v<T, defs::Projectile>)
		{
			appendix += "projectiles/" + subDirectory + "projectile_";
		}
		else if (std::is_base_of_v<T, defs::Controls>)
		{
			appendix += "settings/" + subDirectory;
		}

		return appendix + fileName + ".json"; // .substr(name.find("_") + 1);
	}

	template<>
	std::shared_ptr<defs::Ship> load(const std::string &name)
	{
		std::string filename = makeName<defs::Ship>(name);
    	auto f = std::make_shared<defs::Ship>();
		std::ifstream file(filename);
		if(!file.is_open())
		{
      		throw("not found: " + name);
    	};
		json j;
		try
		{
			file >> j;
			*f = j.get<defs::Ship>();
			return f;
		}
		catch (std::exception e)
		{
			std::cerr << e.what();
		}
	};

	template<>
	std::shared_ptr<defs::Weapon> load(const std::string& name)
	{
		std::string filename = makeName<defs::Weapon>(name);
		auto f = std::make_shared<defs::Weapon>();
		std::ifstream file(filename);
		if (!file.is_open())
		{
			throw("not found: " + name);
		};
		json j;
		try
		{
			file >> j;
			*f = j.get<defs::Weapon>();
			return f;
		}
		catch (std::exception e)
		{
			std::cerr << e.what();
		}

		return nullptr;
	};

	template<>
	std::shared_ptr<defs::Projectile> load(const std::string &name)
	{
		std::string filename = makeName<defs::Projectile>(name);
		auto f = std::make_shared<defs::Projectile>();
		std::ifstream file(filename);
		if (!file.is_open())
		{
			throw("not found: " + name);
		};
		json j;
		try
		{
			file >> j;
			*f = j.get<defs::Projectile>();
			return f;
		}
		catch (std::exception e)
		{
			std::cerr << e.what();
		}
	};

	template<>
	std::shared_ptr<defs::Controls> load(const std::string& name)
	{
		std::string filename = makeName<defs::Controls>(name);
		auto f = std::make_shared<defs::Controls>();
		std::ifstream file(filename);
		if (!file.is_open())
		{
			throw("not found: " + name);
		};
		json j;
		try
		{
			file >> j;
			*f = j.get<defs::Controls>();
			return f;
		}
		catch (std::exception e)
		{
			std::cerr << e.what();
		}
	};

	template<>
	std::shared_ptr<defs::GameShape> load(const std::string& name)
	{
		std::string filename = makeName<defs::GameShape>(name);
		auto f = std::make_shared<defs::GameShape>();
		std::ifstream file(filename);
		if (!file.is_open())
		{
			throw("not found: " + name);
		};
		json j;
		try
		{
			file >> j;
			*f = j.get<defs::GameShape>();
			return f;
		}
		catch (std::exception e)
		{
			std::cerr << e.what();
		}
	};
}