#include "ship.h"
#include <engine.h>
#include "projectile.h"
#include "system_physics.h"
#include "../components/cmp_particlesystem.h"
#include "../components/cmp_health_component.h"
#include "../components/cmp_weapon.h"
#include "../engine/game_resources.h"
#include <math.h>

using namespace sf;

Ship::Ship(Scene* const s, std::string shipDefinition) : Pawn(s)
{
	// Get the ship definition
	_shipDefinition = Resources::get<defs::Ship>(shipDefinition);
	std::shared_ptr<defs::GameShape> _shipShape = Resources::get<defs::GameShape>(_shipDefinition->shape);
	std::shared_ptr<defs::GameShape> _shieldShape = Resources::get<defs::GameShape>("shield");

	// Setup collision filters for the entity
	b2FixtureDef FixtureDef;
	FixtureDef.filter.categoryBits = Physics::COLLISION_DYNAMIC;
	FixtureDef.filter.maskBits = Physics::MASK_DYNAMIC;

	// Setup ship movement, health and weapon components based on ship definition
	movementComponent = addComponent<PlanePhysicsComponent>(_shipDefinition, FixtureDef);
	healthComponent = addComponent<HealthComponent>(*_shipDefinition.get());
	healthComponent->onShieldHit = FFloatDelegate::from_function<Ship, &Ship::OnShieldHit>(this);
	healthComponent->onHullHit = FFloatDelegate::from_function<Ship, &Ship::OnHullHit>(this);

	for (auto& w : _shipDefinition->weapons)
	{
		auto newWeapon = addComponent<WeaponComponent>(w);
		newWeapon->getShape().setFillColor(sf::Color::Black);
		newWeapon->getShape().setOutlineColor(sf::Color::White);
		newWeapon->getShape().setOutlineThickness(2.0f);
		weaponComponents.push_back(newWeapon);
	}

	// Thruster Particle System
	thrusterPS = addComponent<ThrusterParticleSystem>();

	// Setup ship shape
	shapeComponent = addComponent<ShapeComponent>();
	shapeComponent->setShape<sf::Shape>(_shipShape->getShape());
	sf::ConvexShape* shape = (sf::ConvexShape*)&shapeComponent->getShape();
	shape->setFillColor(sf::Color::Black);
	shape->setOutlineColor(sf::Color::White);
	shape->setOutlineThickness(2.0f);

	float shieldScale = std::max(shape->getLocalBounds().width, shape->getLocalBounds().height);

	// Setup shield shape
	_shieldShape->scale = shieldScale;

	shieldComponent = addComponent<ShapeComponent>();
	shieldComponent->setShape<sf::Shape>(_shieldShape->getShape());
	shieldComponent->setLayer(2);
	shape = (sf::ConvexShape*)&shieldComponent->getShape();
	shape->setFillColor(sf::Color::Transparent);
	shape->setOutlineColor(sf::Color::Cyan);
	shape->setOutlineThickness(2.0f);
	//shape->setScale(shieldScale, shieldScale);
}

void Ship::update(double dt)
{
	// Setup correct particle system offset and fire if we're accelerating
	thrusterPS->setOffset(getPosition() - movementComponent->getForwardVector());

	if (movementComponent->isAccelerating())
	{
		thrusterPS->fire();
	}

	if (_shieldVisibility > 0.0f)
	{
		_shieldVisibility -= dt;
	}

	sf::Color shieldColor = shieldComponent->getShape().getOutlineColor();
	shieldColor.a = (_shieldVisibility > 0 ? _shieldVisibility : 0) * 255;
	shieldComponent->getShape().setOutlineColor(shieldColor);

	if (_isFiring)
	{
		Fire();
	}

	Pawn::update(dt);
}

void Ship::render()
{
	Pawn::render();
}

void Ship::OnHit(float damage)
{
	// Pass the damage to health component
	healthComponent->onHit(damage);
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
			// Set weapon rotation to entities. Might wanna move that elsewhere for AI aiming
			weapon->setRotation(getRotation());
			weapon->fire();
		}
	}
}

void Ship::OnShieldHit(float damage)
{
	std::cout << "Shield hit! Damage: " << damage << "   " << GetHealth() << std::endl;

	_shieldVisibility = 1.0f;
	// Draw shiedl effect for a frame
}

void Ship::OnHullHit(float damage)
{
	std::cout << "Hull hit! Damage: " << damage << "   " << GetHealth() << std::endl;

	// Destroy ship when at 0hp
	if (healthComponent->getCurrentHealth() <= 0.0f && !_godMode)
	{
		OnDestroyed();
		setForDelete();
	}
}

std::string Ship::GetHealth()
{
	return "Hull: " + std::to_string(healthComponent->getCurrentHealth()) + "/" + std::to_string(healthComponent->getMaxHealth()) + "Shield: " +
		std::to_string(healthComponent->getCurrentShield()) + "/" + std::to_string(healthComponent->getMaxShield());
}
