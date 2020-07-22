#pragma once
#include <ecm.h>
#include "../components/cmp_bullet.h"
#include "../components/cmp_sprite.h"

class Projectile : public Entity
{
public:
	Projectile() = delete;
	Projectile(Scene* const s, Entity* const owner);

	void update(double) override;
	void fire(sf::Vector2f direction);

protected:
	Entity* _owner;

	float _damage;

	std::shared_ptr<ShapeComponent> shapeComponent;
	std::shared_ptr<BulletComponent> bulletComponent;
};

