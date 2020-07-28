#pragma once
#include <ecm.h>
#include <SFML/Graphics/Shader.hpp>
#include "cmp_sprite.h"
#include "system_resources.h"
#include "system_renderer.h"

class Particle : public ShapeComponent
{
public:
	Particle() : ShapeComponent(nullptr)
	{

	}

	void operator = (const Particle& p)
	{
		startSize = p.startSize;
		endSize = p.endSize;
		lifetime = p.lifetime;
		startColor = p.startColor;
		endColor = p.endColor;
		_drawable->layer = p._drawable->layer;
	}

	explicit Particle(Entity* p) : ShapeComponent(p)
	{
	}

	void reset(sf::Vector2f position)
	{
		_position = position;
		setShape<sf::RectangleShape>(startSize);
		getShape().setOrigin(startSize / 2.0f);
		remainingLifetime = lifetime;
	}

	virtual void render() override
	{
		if (remainingLifetime > 0.0f)
		{
			Renderer::queue(_drawable.get());
		}
	}

	virtual void update(double dt) override
	{
		remainingLifetime -= dt;
		if (remainingLifetime <= 0.0f)
		{
			return;
		}

		_position += (float)dt * velocity;

		_shape->setPosition(_position);
		_shape->setRotation(_rotation);
		getShape().scale(lerp<sf::Vector2f>(sf::Vector2f(endSize.x / startSize.x, endSize.y / startSize.y), sf::Vector2f(1.0f, 1.0f), getPercentLifetime()));

		sf::Color currentColor;
		currentColor.r = lerp<float>((float)endColor.r, (float)startColor.r, getPercentLifetime());
		currentColor.g = lerp<float>((float)endColor.g, (float)startColor.g, getPercentLifetime());
		currentColor.b = lerp<float>((float)endColor.b, (float)startColor.b, getPercentLifetime());
		currentColor.a = 255;
		getShape().setFillColor(currentColor);
	}

	float getPercentLifetime() { return remainingLifetime / lifetime; }

	float lifetime;
	float remainingLifetime;

	std::shared_ptr<sf::Shader> _shader;

	sf::Vector2f velocity;

	sf::Vector2f startSize;
	sf::Vector2f endSize;

	sf::Color startColor;
	sf::Color endColor;

protected:
	sf::Vector2f _position;
	float _rotation;
};

class Emitter
{
public:
	Emitter() { }

	Emitter(Particle& base)
	{
		_base = base;
	}

	void setPosition(sf::Vector2f position)
	{
		_position = position;
	}

	void setMaxParticles(int maxParticles) 
	{ 
		_maxParticles = maxParticles; 
		_particles.resize(_maxParticles);
		for (int i = 0; i < _maxParticles; ++i)
		{
			_particles[i] = _base;
		}
	}
	void setEmitterCooldown(float cooldown) { _emitCooldown = cooldown; }
	
	void emit(int count)
	{
		if (_currentEmitCooldown > 0.0f)
		{
			return;
		}

		int liveParticles = 0;
		int n = 0;

		for (Particle &p : _particles)
		{
			if (n >= count || liveParticles >= _maxParticles)
			{
				break;
			}

			if (p.remainingLifetime < 0.0f)
			{
				p.reset(_position);
				++n;
			}

			++liveParticles;
		}

// 		while (n < count && liveParticles < _maxParticles)
// 		{
// 			Particle p = _base;
// 			p.reset(_position);
// 			_particles.push_back(p);
// 			++liveParticles;
// 			++n;
// 		}

		_currentEmitCooldown = _emitCooldown;
	}

	void update(double dt)
	{
		if (_currentEmitCooldown > 0.0f)
		{
			_currentEmitCooldown -= dt;
		}

		for (Particle &p : _particles)
		{
			p.update(dt);
		}
	}

	void render()
	{
		for (Particle &p : _particles)
		{
			p.render();
		}
	}

protected:
	Particle _base;

	int _maxParticles;
	float _emitCooldown;
	float _currentEmitCooldown;

	std::vector<Particle> _particles;
	sf::Vector2f _position;
};

class ParticleSystemComponent : public Component
{
public:
	ParticleSystemComponent() = delete;
	explicit ParticleSystemComponent(Entity* p);

	void setOffset(sf::Vector2f offset) { _offset = offset; }

	virtual void update(double dt) override;
	virtual void render() override;
	virtual void fire() { }

protected:
	sf::Vector2f _offset;
	std::vector<Particle> _particles;
};

class ThrusterParticleSystem : public ParticleSystemComponent
{
public:
	ThrusterParticleSystem() = delete;
	explicit ThrusterParticleSystem(Entity* p)
		: ParticleSystemComponent(p)
	{
		Particle particleBase(nullptr);
		particleBase.startSize = sf::Vector2f(10, 10);
		particleBase.endSize = sf::Vector2f(3, 3);
		particleBase.lifetime = .25f;
		particleBase.velocity = sf::Vector2f(1.0f, 0.0f);
		particleBase.setLayer(-1);
		particleBase.startColor = sf::Color::White;
		particleBase.endColor = sf::Color::Yellow;

		e = Emitter(particleBase);
		e.setMaxParticles(150);
		e.setEmitterCooldown(0.001f);

		_shader = Resources::load<sf::Shader>("basic");

	}

	virtual void update(double dt) override
	{
		e.setPosition(_offset);
		e.update(dt);
	}
	
	virtual void render() override
	{
		e.render();
	}

	virtual void fire()
	{
		e.emit(particlePerBurst);
	}

protected:
	std::shared_ptr<sf::Shader> _shader;
	Emitter e;
	
	int particlePerBurst = 1;
};