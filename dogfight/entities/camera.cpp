#include "camera.h"
#include "engine.h"
#include <system_renderer.h>

Camera::Camera(Scene* const s)
	: Entity(s)
{
	_view = sf::View(sf::FloatRect(0.0f, 0.0f, Engine::getWindowSize().x, Engine::getWindowSize().y));
}

void Camera::update(double dt)
{
	sf::Vector2f position = getPosition();
	sf::Vector2f newPosition = { 0.0f, 0.0f };

	// Calculate average position
	int n = 0;
	for(auto e : _entitiesToFollow)
	{
		newPosition += e->getPosition();
		++n;
	}

	newPosition = newPosition / (float)n;

	// Interpolate between old and new
	setPosition(lerp<sf::Vector2f>(position, newPosition, 0.01f));
	_view.setCenter(getPosition());

	Engine::GetWindow().setView(_view);
}