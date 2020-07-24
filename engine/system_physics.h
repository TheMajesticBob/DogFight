#pragma once

#include "Box2D/Box2D.h"
#include <SFML/System/Vector2.hpp>
#include <ecm.h>

// This class is used to handle collision calls
class ContactListener : public b2ContactListener
{
	// Called when bodies begin contact
	void BeginContact(b2Contact* contact)
	{
		// User data is set in cmp_physics - it will always be the owner of the Physics Body, therefore we can cast to Entity*
		Entity* entityA = static_cast<Entity*>(contact->GetFixtureA()->GetBody()->GetUserData());
		Entity* entityB = static_cast<Entity*>(contact->GetFixtureB()->GetBody()->GetUserData());

		// Call BeginOverlap on entities if they are valid
		if (entityA)
		{
			entityA->OnBeginOverlap(entityB);
		}
		if (entityB)
		{
			entityB->OnBeginOverlap(entityA);
		}
	}

	void EndContact(b2Contact* contact)
	{
		// User data is set in cmp_physics - it will always be the owner of the Physics Body, therefore we can cast to Entity*
		Entity* entityA = static_cast<Entity*>(contact->GetFixtureA()->GetBody()->GetUserData());
		Entity* entityB = static_cast<Entity*>(contact->GetFixtureB()->GetBody()->GetUserData());

		// Call OnEndOverlap on entities if they are valid
		if (entityA)
		{
			entityA->OnEndOverlap(entityB);
		}
		if (entityB)
		{
			entityB->OnEndOverlap(entityA);
		}
	}
};

namespace Physics 
{
	// Setup collision filters (physics object types)
	const uint16 COLLISION_STATIC	= 0x0001;
	const uint16 COLLISION_BULLET	= 0x0002;
	const uint16 COLLISION_DYNAMIC	= 0x0004;

	// Setup collision masks
	const uint16 MASK_PROJECTILE = COLLISION_DYNAMIC | COLLISION_STATIC;
	const uint16 MASK_DYNAMIC = COLLISION_DYNAMIC | COLLISION_STATIC;

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