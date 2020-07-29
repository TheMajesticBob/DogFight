#include "planet.h"
#include "../components/cmp_physics.h"
#include "../components/cmp_sprite.h"
#include <system_physics.h>
#include "Box2D/Collision/Shapes/b2CircleShape.h"

Planet::Planet(Scene* const s, float radius, float mass) : Entity(s), _radius(radius), _mass(mass)
{
	b2FixtureDef fixture;
	b2CircleShape shape;

	shape.m_radius = radius * Physics::physics_scale_inv;
	shape.m_p.Set(0, 0);

	physicsComponent = addComponent<PhysicsComponent>(false, shape, fixture);

	shapeComponent = addComponent<ShapeComponent>();
	shapeComponent->setShape<sf::CircleShape>(_radius, 100);
	shapeComponent->getShape().setFillColor(sf::Color(0, 0, 0, 0));
	shapeComponent->getShape().setOutlineColor(sf::Color(255, 0, 255, 255));
	shapeComponent->getShape().setOutlineThickness(2);
	shapeComponent->getShape().setOrigin(sf::Vector2f(radius, radius));//  / 2.0f);
}

void Planet::update(double dt)
{
	b2World* m_world = Physics::GetWorld().get();
	b2Body* m_body = physicsComponent->getFixture()->GetBody();

	for (b2Body* b = m_world->GetBodyList(); b; b = b->GetNext()) 
	{
		if (b == m_body)
		{
			continue;
		}

		float magnitude = sf::length(Physics::bv2_to_sv2(m_body->GetPosition() - b->GetPosition()));
		magnitude *= magnitude;

		b2Vec2 forceDirection = Physics::sv2_to_bv2(sf::normalize(Physics::bv2_to_sv2(m_body->GetPosition() - b->GetPosition())));

		float force = _radius * _mass / magnitude * b->GetGravityScale();
		b->ApplyForceToCenter(force * forceDirection, true);
	}
}
