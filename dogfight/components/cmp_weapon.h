#pragma once
#include "cmp_sprite.h"
#include "../definitions.h"

// A component 
class WeaponComponent : public ShapeComponent 
{
public:
	// Will check the keyboard and move the component's parent.
	void update(double) override;

	void render() override;
	// Used to create the component for an entity
	explicit WeaponComponent(Entity* p, defs::WeaponSlot& weaponSlotDefinition);
	WeaponComponent() = delete;

	void fire(sf::Vector2f directionVector);
	void setRelativePosition(sf::Vector2f relativePosition) { _relativePosition = relativePosition; }

protected:
	std::shared_ptr<defs::Weapon> _weaponDefinition;
	sf::Vector2f _relativePosition;
	
	float _fireCooldown;
};