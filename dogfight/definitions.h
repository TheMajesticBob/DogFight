#pragma once
#include "nlohmann/json.hpp"
#include <SFML/Graphics.hpp>

using nlohmann::json;

namespace sf
{
	inline void from_json(const json &j, Vector2f &v)
	{
		j.at("X").get_to(v.x);
		j.at("Y").get_to(v.y);
	}
};

namespace defs
{
	struct Shape;

	struct Plane
	{
		Plane() {}

		float health;
		float shield;

		std::string shape;

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

		float initialSpeed;
		float linearDamping;
		float lifetime;

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

	struct GameShape
	{
		GameShape() {}

		enum Type
		{
			Rectangle = 0,
			Circle,
			Polygon
		};

		Type type;

		float scale;
		float radius;
		sf::Vector2f size;
		std::vector<sf::Vector2f> points;

		std::shared_ptr<sf::Shape> getShape()
		{
			std::shared_ptr<sf::Shape> returnShape;

			switch (type)
			{
			case Type::Rectangle:
				returnShape.reset(new sf::RectangleShape(size * scale));
				break;

			case Type::Circle:
				returnShape.reset(new sf::CircleShape(radius * scale, 100));
				break;

			case Type::Polygon:
			default:
				returnShape.reset(new sf::ConvexShape(points.size()));
				sf::ConvexShape* shape = (sf::ConvexShape*)returnShape.get();

				for (int i = 0; i < points.size(); ++i)
				{
					shape->setPoint(i, points[i] * scale);
				}
				break;
			}

			return returnShape;
		}
	};

	inline void from_json(const json &j, Plane &p)
	{
		j.at("Health").at("Hull").get_to(p.health);
		j.at("Health").at("Shield").get_to(p.shield);
		j.at("Weapons").get_to(p.weapons);

		j.at("Shape").get_to(p.shape);

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
		j.at("Projectile").at("Lifetime").get_to(p.lifetime);
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

	inline void from_json(const json &j, GameShape &s)
	{
		if(j.find("Circle") != j.end())
		{
			s.type = GameShape::Circle;
			j.at("Circle").get_to(s.radius);
		}
		else if (j.find("Rectangle") != j.end())
		{
			s.type = GameShape::Rectangle;
			s.size = j.at("Rectangle").get<sf::Vector2f>();
		}
		else if (j.find("Polygon") != j.end())
		{
			s.type = GameShape::Polygon;
			j.at("Polygon").get_to<std::vector<sf::Vector2f>>(s.points); // (s.points);

		}

		j.at("Scale").get_to(s.scale);
	}
}