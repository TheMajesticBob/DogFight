#pragma once

#include <SFML/Graphics.hpp>
//#include "cmp_sprite.h"

struct FDrawable
{
	std::shared_ptr<sf::Drawable> drawable;
	int layer;

	FDrawable(std::shared_ptr<sf::Drawable> s, int layer = 0) : drawable(s), layer(layer)
	{

	}
};

namespace Renderer {

	struct FPostProcessEffect
	{
	public:
		FPostProcessEffect()
		{
			set_for_delete = false;
		}

		FPostProcessEffect(sf::Shader* effect) : effect(effect)
		{
			set_for_delete = false;
		}

		const bool operator==(const FPostProcessEffect& rhs) const 
		{
			return effect == rhs.effect;
		}

		sf::Shader* effect;
		bool set_for_delete;
	};

void initialise(sf::RenderWindow&);

void shutdown();

void update(const double&);

void queue(const sf::Drawable* s, int layer = 0);

void queue(const FDrawable* s);

void add_postprocess_effect(FPostProcessEffect& effect);

void remove_postprocess_effect(FPostProcessEffect& effect);

void render();
}; // namespace Renderer
