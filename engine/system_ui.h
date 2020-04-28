#pragma once

#include <SFML/Graphics.hpp>

namespace UI
{
	void shutdown();
	void update(const double&);
	void queue(const sf::Drawable* s);
	void render(sf::RenderTexture& rt);

}
