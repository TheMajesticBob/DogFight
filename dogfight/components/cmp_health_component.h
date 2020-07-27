#pragma once
#include <ecm.h>

// A component 
class HealthComponent : public Component {
protected:
	float _currentHealth;
	float _maxHealth;

	float _regenPerSecond;

public:
	// Will check the keyboard and move the component's parent.
	void update(double) override;
	// Component does not need rendered.
	void render() override {}
	// Used to create the component for an entity
	explicit HealthComponent(Entity* p);
	HealthComponent() = delete;

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
};