#pragma once

#include "Box2D/Box2D.h"
#include <SFML/System/Vector2.hpp>

namespace Physics {

	const uint16 COLLISION_PLAYER	= 0x0001;
	const uint16 COLLISION_ENEMY	= 0x0002;
	const uint16 COLLISION_BULLET	= 0x0004;
	const uint16 COLLISION_STATIC	= 0x0008;

	const uint16 MASK_PROJECTILE = COLLISION_ENEMY | COLLISION_PLAYER | COLLISION_STATIC;
	const uint16 MASK_PLAYER = COLLISION_ENEMY | COLLISION_PLAYER | COLLISION_STATIC;
	const uint16 MASK_ENEMY = COLLISION_BULLET | COLLISION_ENEMY | COLLISION_PLAYER | COLLISION_STATIC;

void initialise();
void shutdown();
void update(const double&);

std::shared_ptr<b2World> GetWorld();

const float physics_scale = 30.0f;
const float physics_scale_inv = 1.0f / physics_scale;

const sf::Vector2f bv2_to_sv2(const b2Vec2& in, bool scale = true);
const b2Vec2 sv2_to_bv2(const sf::Vector2f& in, bool scale = true);
const sf::Vector2f invert_height(const sf::Vector2f& in);
} // namespace Physics
