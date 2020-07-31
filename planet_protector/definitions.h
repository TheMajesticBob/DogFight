#pragma once
#include "nlohmann/json.hpp"
#include <Box2d/Collision/Shapes/b2Shape.h>
#include <Box2d/Collision/Shapes/b2PolygonShape.h>
#include <Box2d/Collision/Shapes/b2CircleShape.h>
#include <SFML/Graphics.hpp>
#include <system_physics.h>

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
	struct WeaponSlot
	{
		WeaponSlot() {}

		std::string weaponName;
		sf::Vector2f relativePosition;
	};

	struct Ship
	{
		Ship() {}

		float health;
		float shield;

		std::string shape;

		std::vector<WeaponSlot> weapons;

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
		std::string shape;
		std::string sound;
		
		float damage;
		float rateOfFire;
		float projectilesPerRound;
	};

	struct Projectile
	{
		Projectile() {}

		sf::Vector2f size;

		std::string shape;

		float initialSpeed;
		float initialRotation = 0;
		float linearDamping;
		float lifetime;

		float mass;

		bool collideWithProjectiles;
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

	struct EnemyGroup
	{
		EnemyGroup() {}

		std::string shipName;
		std::string aiType;
		int count;
	};

	struct Spawner
	{
		Spawner() {}

		sf::Vector2f position;
		std::vector<EnemyGroup> enemyGroups;
		float delayBetweenSpawns;
		float initialDelay;
		float currentDelay = 0;
		int maxShipsPerSpawn;
	};

	struct Wave
	{
		Wave() {}
		Wave(const Wave& w)
		{
			name = w.name;
			spawners = w.spawners;
			delay = w.delay;
		}

		std::string name;
		std::vector<Spawner> spawners;
		float delay;
	}; 
	
	struct Level
	{
		Level() {}

		float planetRadius;
		float planetMass;

		std::vector<Wave> waves;
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
		int sides;
		float radius;
		sf::Vector2f size;
		sf::Vector2f origin;

		std::vector<sf::Vector2f> points;

		b2Shape* getPhysicsShape()
		{
			b2Shape* returnShape;

			switch (type)
			{
			case Type::Rectangle:
			{
				returnShape = new b2PolygonShape();
				b2PolygonShape* polygonShape = (b2PolygonShape*)returnShape;
				polygonShape->SetAsBox(size.x * scale * Physics::physics_scale_inv, size.y * scale * Physics::physics_scale_inv, b2Vec2_zero, 0);

			}
				break;

			case Type::Circle:
			{
				returnShape = new b2CircleShape();
				b2CircleShape* circleShape = (b2CircleShape*)returnShape;
				circleShape->m_radius = radius * scale * Physics::physics_scale_inv;
			}
				break;

			case Type::Polygon:
			default:
				{
					returnShape = new b2PolygonShape();
					b2PolygonShape* polygonShape = (b2PolygonShape*)returnShape;

					// Set triangle vertices
					std::vector<b2Vec2> vertices;
					for (int i = 0; i < points.size(); ++i)
					{
						// We have to scale the points properly by using sv2_to_bv2
						b2Vec2 point = scale * Physics::sv2_to_bv2(points[i] - origin);
						vertices.push_back(b2Vec2(point.x, point.y));
					}

					polygonShape->Set(&vertices[0], points.size());
				}
				break;
			}

			return returnShape;
		}

		std::shared_ptr<sf::Shape> getShape()
		{
			std::shared_ptr<sf::Shape> returnShape;

			switch (type)
			{
			case Type::Rectangle:
				returnShape.reset(new sf::RectangleShape(size * scale * 2.0f));
				break;

			case Type::Circle:
				returnShape.reset(new sf::CircleShape(radius * scale, sides));
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

			returnShape->setOrigin(origin * scale);
			return returnShape;
		}
	};

	inline void from_json(const json &j, WeaponSlot &ws)
	{
		j.at("Name").get_to(ws.weaponName);
		j.at("Position").get_to(ws.relativePosition);
	}

	inline void from_json(const json &j, Ship &p)
	{
		j.at("Health").at("Hull").get_to(p.health);
		j.at("Health").at("Shield").get_to(p.shield);
		j.at("Weapons").get_to<std::vector<WeaponSlot>>(p.weapons);

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
		j.at("Sound").get_to(w.sound);
		j.at("Shape").get_to(w.shape);
		j.at("Damage").get_to(w.damage);
		j.at("RateOfFire").get_to(w.rateOfFire);
		j.at("ProjectilesPerRound").get_to(w.projectilesPerRound);
	}

	inline void from_json(const json &j, Projectile &p)
	{
		j.at("Projectile").at("Size").at("X").get_to(p.size.x);
		j.at("Projectile").at("Size").at("Y").get_to(p.size.y);
		j.at("Projectile").at("InitialSpeed").get_to(p.initialSpeed);
		j.at("Projectile").at("CollideWithProjectiles").get_to(p.collideWithProjectiles);
		if (j.at("Projectile").find("InitialRotation") != j.at("Projectile").end())
		{
			j.at("Projectile").at("InitialRotation").get_to(p.initialRotation);
		}
		j.at("Projectile").at("Lifetime").get_to(p.lifetime);
		j.at("Projectile").at("Shape").get_to(p.shape);
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

	inline void from_json(const json &j, EnemyGroup &eg)
	{
		j.at("ShipType").get_to(eg.shipName);
		j.at("AIType").get_to(eg.aiType);
		j.at("Amount").get_to(eg.count);
	}

	inline void from_json(const json &j, Spawner &s)
	{
		j.at("DelayBetweenSpawns").get_to(s.delayBetweenSpawns);
		if (j.find("InitialDelay") != j.end())
		{
			j.at("InitialDelay").get_to(s.initialDelay);
		}
		j.at("MaxShipsPerSpawn").get_to(s.maxShipsPerSpawn);
		j.at("Ships").get_to(s.enemyGroups);
	}

	inline void from_json(const json &j, Wave &w)
	{
		j.at("Name").get_to(w.name);
		j.at("Delay").get_to(w.delay);
		j.at("Spawners").get_to(w.spawners);
	}

	inline void from_json(const json &j, Level &l)
	{
		j.at("Planet").at("Radius").get_to(l.planetRadius);
		j.at("Planet").at("Mass").get_to(l.planetMass);
		j.at("Waves").get_to(l.waves);
	}
	
	inline void from_json(const json &j, GameShape &s)
	{
		if(j.find("Circle") != j.end())
		{
			s.type = GameShape::Circle;
			j.at("Circle").at("Radius").get_to(s.radius);
			j.at("Circle").at("Sides").get_to(s.sides);
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

		j.at("Origin").get_to(s.origin);
		j.at("Scale").get_to(s.scale);
	}
}