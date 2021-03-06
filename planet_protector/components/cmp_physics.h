#pragma once

#include "cmp_sprite.h"

#include "ecm.h"
#include <system_physics.h>
#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2Fixture.h>

class PhysicsComponent : public Component {
protected:
  b2Body* _body;
  const bool _dynamic;
  b2Fixture* _fixture;
  bool _desiredActive;

public:
	PhysicsComponent(Entity* p, bool dyn);
	PhysicsComponent(Entity* p, bool dyn, b2Shape& Shape, b2FixtureDef& FixtureDef = b2FixtureDef());
	PhysicsComponent(Entity* p, bool dyn, const sf::Vector2f& size, b2FixtureDef& FixtureDef = b2FixtureDef());

	b2Fixture* const getFixture() const;
	bool isTouching(const PhysicsComponent& pc) const;
	bool isTouching(const PhysicsComponent& pc, b2Contact const* bc) const;
	std::vector<const b2Contact*> getTouching() const;
	const sf::Vector2f getVelocity() const;
	void setRestitution(float r);
	void setFriction(float r);
	void setActive(bool active) { _desiredActive = active; }
	void setMass(float m);
	void update(double dt) override;
	void render() override;
	void impulse(const sf::Vector2f& i);
	void dampen(const sf::Vector2f& s);
	void setVelocity(const sf::Vector2f& v);
	void teleport(const sf::Vector2f& v);
	void setRotation(const float rotation);
	~PhysicsComponent() override;
};
