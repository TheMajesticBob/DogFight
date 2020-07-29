#pragma once
#include <ecm.h>
#include "../components/cmp_bullet.h"
#include "../components/cmp_sprite.h"
#include "../definitions.h"
#include "pool.h"
#include "system_physics.h"

class Projectile : public Entity, public Poolable
{
public:
	Projectile() = delete;
	Projectile(Scene* const s);

	void Reset() override;
	void Destroy() override;

	void update(double) override;
	void render() override;

	void fire(std::string definition, Entity* const owner, sf::Vector2f direction);

	void setDamage(float& damage) { _damage = damage; }

	virtual void OnBeginOverlap(Entity* const e) override;

protected:
	std::shared_ptr<ShapeComponent> shapeComponent;
	std::shared_ptr<BulletComponent> bulletComponent;

	std::shared_ptr<defs::Projectile> _projectileDefiniton;

	Entity* _owner;
	float _damage;
};