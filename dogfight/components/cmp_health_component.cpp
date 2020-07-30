#include "cmp_health_component.h"

void HealthComponent::update(double dt)
{
	if (_healthRegen > 0.0f)
	{
		_currentHealth += dt * _healthRegen;
		if (_currentHealth > _maxHealth)
		{
			_currentHealth = _maxHealth;
		}
	}
	if (_shieldRegen > 0.0f)
	{
		_currentShield += dt * _shieldRegen;
		if (_currentShield > _maxShield)
		{
			_currentShield = _maxShield;
		}
	}
}

void HealthComponent::onHit(float damage)
{
	if (_currentShield > 0.0f)
	{
		float shieldLeft = _currentShield - damage;
		if (shieldLeft < 0.0f)
		{
			damage = -shieldLeft;
			_currentShield = 0;
		}
		else 
		{
			_currentShield = shieldLeft;
			damage = 0;
		}

		onShieldHit.invokeSafe(damage);
	}

	if (damage > 0.0f)
	{
		if (_currentHealth > 0.0f)
		{
			_currentHealth -= damage;
		}
		else 
		{
			_currentHealth = 0.0f;
		}

		onHullHit.invokeSafe(damage);
	}
}

HealthComponent::HealthComponent(Entity* p, defs::Ship shipDefinition) : Component(p)
{
	setMaxHealth(shipDefinition.health, true);
	setMaxShield(shipDefinition.shield, true);
}

