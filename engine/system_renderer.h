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


void initialise(sf::RenderWindow&);

void shutdown();

void update(const double&);

void queue(const sf::Drawable* s);

void queue(const FDrawable* s);

void render();
}; // namespace Renderer
