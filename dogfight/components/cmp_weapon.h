#pragma once
#include <ecm.h>
#include "../definitions.h"

// A component 
class WeaponComponent : public Component 
{
public:
	// Will check the keyboard and move the component's parent.
	void update(double) override;
	// Component does not need rendered.
	void render() override {};
	// Used to create the component for an entity
	explicit WeaponComponent(Entity* p, std::string weaponDefinition);
	WeaponComponent() = delete;

	void fire(sf::Vector2f directionVector);


protected:
	std::shared_ptr<defs::Weapon> _weaponDefinition;
	
	float _fireCooldown;
};