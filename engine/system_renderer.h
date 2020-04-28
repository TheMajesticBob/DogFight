#pragma once

#include <SFML/Graphics.hpp>
#include "system_ui.h"
//#include "cmp_sprite.h"

namespace Renderer {
void initialise(sf::RenderWindow&);

void shutdown();

void update(const double&);

void queue(const sf::Drawable* s);

void render();
}; // namespace Renderer
