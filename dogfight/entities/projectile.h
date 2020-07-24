#pragma once
#include <ecm.h>
#include "../components/cmp_bullet.h"
#include "../components/cmp_sprite.h"
#include "system_physics.h"

class Projectile : public Entity
{
public:
	Projectile() = delete;
	Projectile(Scene* const s, Entity* const owner);

	void update(double) override;
	void fire(sf::Vector2f direction);

	virtual void OnBeginOverlap(Entity* const e) override;

protected:
	Entity* _owner;

	float _damage;

	std::shared_ptr<ShapeComponent> shapeComponent;
	std::shared_ptr<BulletComponent> bulletComponent;
};