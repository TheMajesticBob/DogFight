#pragma once
#include <ecm.h>

class Planet : public Entity
{
public:
	Planet() = delete;
	Planet(Scene* const s, float radius, float mass);

	std::shared_ptr<class PhysicsComponent> GetPhysicsComponent() { return physicsComponent; }

	float getRadius() { return _radius; }
	float getMass() { return _mass; }

	void update(double dt) override;

protected:
	std::shared_ptr<class PhysicsComponent> physicsComponent;
	std::shared_ptr<class ShapeComponent> shapeComponent;

	float _radius;
	float _mass;
};