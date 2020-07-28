#pragma once
#include <ecm.h>
#include "../components/cmp_bullet.h"
#include "../components/cmp_sprite.h"
#include "../definitions.h"
#include "system_physics.h"

class Projectile : public Entity
{
public:
	Projectile() = delete;
	Projectile(Scene* const s, std::string definition, Entity* const owner);

	void update(double) override;
	void render() override;

	void fire(sf::Vector2f direction);

	void setDamage(float& damage) { _damage = damage; }

	virtual void OnBeginOverlap(Entity* const e) override;

protected:
	Entity* _owner;
	float _damage;

	std::shared_ptr<ShapeComponent> shapeComponent;
	std::shared_ptr<BulletComponent> bulletComponent;

	std::shared_ptr<defs::Projectile> _projectileDefiniton;
};