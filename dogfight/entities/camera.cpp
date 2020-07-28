#include "camera.h"
#include "engine.h"
#include <system_renderer.h>

Camera::Camera(Scene* const s)
	: Entity(s)
{
	_view = sf::View(sf::FloatRect(0.0f, 0.0f, Engine::getWindowSize().x, Engine::getWindowSize().y));
	_viewSize = _view.getSize();
}

void Camera::update(double dt)
{
	sf::FloatRect bounds;

	sf::Vector2f position = getPosition();
	sf::Vector2f newPosition = { 0.0f, 0.0f };

	float maxDistance = 0;

	// Calculate average position
	int n = 0;
	for(auto e : _entitiesToFollow)
	{
		sf::Vector2f entityPosition = e->getPosition();
		float distance = sf::length(position - entityPosition);
		if (distance > maxDistance)
		{
			maxDistance = distance;
		}
		newPosition += entityPosition;
		++n;
	}

	newPosition = newPosition / (float)n;

	// Interpolate between old and new
	setPosition(lerp<sf::Vector2f>(position, newPosition, 0.01f));
	_view.setCenter(getPosition());
	if (maxDistance > distToZoom)
	{
		float s = maxDistance / distToZoom;
		_view.setSize(s * _viewSize);
	}

	Engine::GetWindow().setView(_view);
}