#pragma once
//#include "system_renderer.h"
#include <SFML/Graphics.hpp>

struct FDrawable;

namespace UI
{
	void shutdown();
	void update(const double&);
	void queue(const sf::Drawable* s);
	void queue(const FDrawable* s);
	void render(sf::RenderTexture& rt);

}
