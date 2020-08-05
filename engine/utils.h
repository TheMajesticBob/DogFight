#pragma once
#include <SFML/Graphics/View.hpp>

namespace Utils
{
	sf::Vector2f WorldToScreen(const sf::Vector2f& position);
	sf::Vector2f ScreenToWorld(const sf::Vector2f& position);
}