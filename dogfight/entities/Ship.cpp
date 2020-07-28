#include "ship.h"
#include <engine.h>
#include "projectile.h"
#include "system_physics.h"
#include "../components/cmp_health_component.h"
#include "../components/cmp_weapon.h"
#include "../engine/game_resources.h"
#include "GL/glew.h"

using namespace sf;

Ship::Ship(Scene* const s, std::string shipDefinition) : Pawn(s)
{
	// Get the ship definition
	_shipDefinition = Resources::get<defs::Plane>(shipDefinition);

	// Size?
	Vector2f size = { 30.0f, 30.0f };

	// Setup collision filters for the entity
	b2FixtureDef FixtureDef;
	FixtureDef.filter.categoryBits = Physics::COLLISION_DYNAMIC;
	FixtureDef.filter.maskBits = Physics::MASK_DYNAMIC;

	// Setup ship movement, health and weapon components based on ship definition
	movementComponent = addComponent<PlanePhysicsComponent>(size, _shipDefinition, FixtureDef);
	healthComponent = addComponent<HealthComponent>(_shipDefinition->health);
	for (auto& w : _shipDefinition->weapons)
	{
		weaponComponents.push_back(addComponent<WeaponComponent>(w));
	}

	// Thruster shape lol its ugly af. TODO: Replace this shiiiit
	thrusterComponent = addComponent<ShapeComponent>();
	thrusterComponent->setShape<RectangleShape>(Vector2f(size.x, size.y / 2.0f));
	thrusterComponent->getShape().setFillColor(Color::Yellow);
	thrusterComponent->getShape().setOrigin(Vector2f(size.x / 2.0f, size.y / 4.0f));
	thrusterComponent->setVisibility(false);
	thrusterComponent->setLayer(-1);

	// Setup draw shape
	// Right vertex
	drawVerts[0] = 0.0f;
	drawVerts[1] = -size.x / 2.0f;

	// Top vertex
	drawVerts[2] = -size.x * sqrt(3) / 2.0f;
	drawVerts[3] = 0; // -1.0f * Physics::physics_scale;

	// Left vertex
	drawVerts[4] = 0;
	drawVerts[5] = size.x / 2.0f;

	// Bottom vertex
	drawVerts[6] = -size.x * sqrt(3) / 12.0f;
	drawVerts[7] = 0;

	for (int i = 0; i < 8; ++i)
	{
		drawVerts[i] = drawVerts[i];
	}

	// Currently this is how we draw the ship
	movementComponent->setDebugDraw(false);

	// The following does not match the actual triangle shape of movement component, therefore I left the drawing to it
	// We should definitely look into that when we get assets
// 	float radius = size.x / 2.0f;
// 
// 	shapeComponent = addComponent<ShapeComponent>();
// 	shapeComponent->setShape<CircleShape>(radius, 30);
// 	shapeComponent->getShape().setFillColor(Color::Blue);
// 	shapeComponent->getShape().setOrigin(Vector2f(0.0f, 0.0f));
}

void Ship::update(double dt)
{
	// Update thruster visibility based on whether the ship is accelerating
	// TODO: Replace with particle system
	// thrusterComponent->setVisibility(movementComponent->isAccelerating());

	Pawn::update(dt);
}

void Ship::render()
{
	Engine::GetWindow().pushGLStates();
	Pawn::render();
	Engine::GetWindow().popGLStates();

	glColor3f(1, 0, 0);
	glPushMatrix();
	glTranslatef(getPosition().x, getPosition().y, 0);
	glRotatef(-getRotation(), 0, 0, 1);
	
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_TEXTURE_2D);

	GLfloat colors[] =
	{
		color.r, color.g, color.b,
		color.r, color.g, color.b,
		color.r, color.g, color.b,
		color.r, color.g, color.b
	};

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, drawVerts);
	//glDisableClientState(GL_VERTEX_ARRAY);

	//glColor4f(0.0f, 0.0f, 0.0f, 1);
	//glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		glLineWidth(2);
		//glColor4f(color.r, color.g, color.b, 1);
		// glColor4f(1, 0, 0, 1);
		glColor3f(1, 0, 0);
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(3, GL_FLOAT, 0, colors);
		//glDisableClientState(GL_COLOR_ARRAY);
		glDrawArrays(GL_LINE_LOOP, 0, 4);

// 		glColor3f(1, 1, 1);//white
// 
// 
// 		//circle outline
// 		glBegin(GL_LINE_LOOP);
// 		for (float a = 0; a < deg2rad(360); a += deg2rad(30))
// 		{
// 			sf::Vector2f vert = Physics::bv2_to_sv2(b2Vec2(sinf(a), cosf(a)));
// 			glVertex2f(vert.x, vert.y);
// 		}
// 		glEnd();

	glPopMatrix();

	// movementComponent->getFixture()->GetShape().
}

void Ship::OnHit(float damage)
{
	// Pass the damage to health component
	healthComponent->onHit(damage);

	// Destroy ship when at 0hp
	if (healthComponent->getCurrentHealth() <= 0.0f)
	{
		OnDestroyed();
		setForDelete();
	}
}

void Ship::OnDestroyed()
{
	// TODO: Make Boom!
}

void Ship::Accelerate(float Value)
{
	movementComponent->accelerate(Value);
}

void Ship::Turn(float Value)
{
	movementComponent->turn(Value);
}

void Ship::Fire()
{
	for (auto weapon : weaponComponents)
	{
		if (weapon)
		{
			weapon->fire(movementComponent->getForwardVector());
		}
	}
}