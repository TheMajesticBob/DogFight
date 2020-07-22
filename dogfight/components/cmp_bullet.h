#pragma once
#include "cmp_physics.h"
#include "../definitions.h"
#include <ecm.h>

class BulletComponent : public PhysicsComponent 
{
protected:
	b2Vec2 _size;

	float _lifetime;
	float _initialSpeed;
	sf::Vector2f _velocity;

	bool _affectedByGravity;

	std::shared_ptr<defs::Projectile> _projectileDefinition;

public:
	void fire(sf::Vector2f& direction);

	void update(double dt) override;
	void render() override {}
	explicit BulletComponent(Entity* p, const sf::Vector2f& size, float lifetime = 3.f, b2FixtureDef& fixtureDef = b2FixtureDef());
	BulletComponent() = delete;
};
