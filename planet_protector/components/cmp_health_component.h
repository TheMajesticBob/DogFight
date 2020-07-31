#pragma once
#include <ecm.h>
#include "../definitions.h"
#include <delegates.h>

DEFINE_DELEGATE(FFloatDelegate, float);

// A component 
class HealthComponent : public Component {
protected:
	float _currentHealth;
	float _maxHealth;

	float _currentShield;
	float _maxShield;

	float _healthRegen;
	float _shieldRegen;

public:
	// Will check the keyboard and move the component's parent.
	void update(double) override;
	// Component does not need rendered.
	void render() override {};
	// Used to create the component for an entity
	explicit HealthComponent(Entity* p, defs::Ship shipDefinition);
	HealthComponent() = delete;

	// Delegates called when hit
	FFloatDelegate onShieldHit;
	FFloatDelegate onHullHit;

	// Called when an entity gets hit
	void onHit(float damage);

	// Health setter
	void setMaxHealth(float maxHealth, bool setCurrent)
	{
		_maxHealth = maxHealth;
		if (setCurrent)
		{
			_currentHealth = maxHealth;
		}
	}
	// Shield setter
	void setMaxShield(float maxShield, bool setCurrent)
	{
		_maxShield = maxShield;
		if (setCurrent)
		{
			_currentShield = maxShield;
		}
	}

	// Health getters
	float getCurrentHealth() { return _currentHealth; }
	float getMaxHealth() { return _maxHealth; }
	float getHealthPercent() { return _currentHealth / _maxHealth; }

	// Health getters
	float getCurrentShield() { return _currentShield; }
	float getMaxShield() { return _maxShield; }
	float getShieldPercent() { return _currentShield / _maxShield; }
};