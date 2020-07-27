#include "cmp_health_component.h"

void HealthComponent::update(double dt)
{
	if (_regenPerSecond > 0.0f)
	{
		_currentHealth += dt * _regenPerSecond;
		if (_currentHealth > _maxHealth)
		{
			_currentHealth = _maxHealth;
		}
	}
}

void HealthComponent::onHit(float damage)
{
	_currentHealth -= damage;
}

HealthComponent::HealthComponent(Entity* p, float maxHealth) : Component(p), _currentHealth(maxHealth), _maxHealth(maxHealth)
{

}

