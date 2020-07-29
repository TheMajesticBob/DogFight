#pragma once
#include "cmp_physics.h"
#include "../definitions.h"
#include <ecm.h>

class BulletComponent : public PhysicsComponent 
{
protected:
	float _lifetime;
	float _initialSpeed;
	sf::Vector2f _velocity;

	bool _affectedByGravity;

	b2FixtureDef _fixtureDef;

	std::shared_ptr<defs::Projectile> _projectileDefinition;

public:
	void fire(const sf::Vector2f& position, sf::Vector2f& direction);
	void redefine(std::shared_ptr<defs::Projectile> definition, b2FixtureDef& fixtureDef = b2FixtureDef());

	void update(double dt) override;
	void render() override {}
	explicit BulletComponent(Entity* p, float lifetime = 3.f, b2FixtureDef& fixtureDef = b2FixtureDef());
	BulletComponent() = delete;
};
