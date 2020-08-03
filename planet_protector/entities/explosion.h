#pragma once
#include <SFML/Graphics.hpp>
#include "../components/cmp_sprite.h"
#include <ecm.h>

struct Explosion
{
	Explosion()
	{
		lifetime = 1.0f;
		currentLifetime = 10.0f;
	}

	void update(double dt)
	{
		currentLifetime += dt;
		if (currentLifetime >= lifetime)
		{
			float pos = -size.y * 2.0f;
			position = sf::Vector2f(pos, pos);
			explosionSprite.setPosition(position);
		}

		const float scale = GetNormalizedLifetime();
		explosionSprite.setScale(scale, scale);
	}

	const float GetNormalizedLifetime() const
	{
		return currentLifetime / lifetime;
	}

	sf::Sprite explosionSprite;

	sf::Vector2f position;
	sf::Vector2f size;
	sf::Vector2f currentSize;
	float lifetime;
	float currentLifetime;
};

class ExplosionManager : public Entity
{
public:
	ExplosionManager() = delete;
	ExplosionManager(class Scene* const s);
	~ExplosionManager();

    // Updates all bullets
    void update(double dt) override;

    // Renders all bullets
    void render() override;

    // Choose an inactive bullet and use it
    void Fire(const sf::Vector2f &pos, const sf::Vector2f &size, const float &lifetime);
    
protected:

	sf::Sprite _textureSprite;
	sf::Shader _shockwaveShader;
	sf::RenderTexture _shockwaveRT;
    
	std::shared_ptr<sf::Texture> _shockwaveTexture;

	Renderer::FPostProcessEffect _ppEffect;

    static unsigned char explosionPointer;
    static Explosion explosions[256];
};
