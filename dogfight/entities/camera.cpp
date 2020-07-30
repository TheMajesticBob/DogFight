#include "camera.h"
#include "engine.h"
#include <system_renderer.h>

Camera::Camera(Scene* const s)
	: Entity(s)
{
	_view = sf::View(sf::FloatRect(0.0f, 0.0f, Engine::getWindowSize().x, Engine::getWindowSize().y));
	_viewSize = _view.getSize();
}

void Camera::update(double)
{
	Engine::GetWindow().setView(_view);
}

void Camera::setPosition(sf::Vector2f position)
{
	Entity::setPosition(position);
	_view.setCenter(position);
}

void Camera::setScale(float scale)
{
	_view.setSize(scale * _viewSize);
}

FollowCamera::FollowCamera(Scene* const s) : Camera(s)
{

}

void FollowCamera::update(double dt)
{
	sf::FloatRect bounds;

	sf::Vector2f position = getPosition();
	sf::Vector2f newPosition = { 0.0f, 0.0f };

	float maxDistance = 0;

	// Calculate average position
	float w = 0;
	for(auto e : _entitiesToFollow)
	{
		if (e.getEntity()->isAlive())
		{
			sf::Vector2f entityPosition = e.getEntity()->getPosition();
			float distance = sf::length(position - entityPosition);
			if (distance > maxDistance)
			{
				maxDistance = distance;
			}
			newPosition += entityPosition * e.getWeight();
			w += e.getWeight();
		}
	}

	newPosition = newPosition / (float)w;

	// Interpolate between old and new
	setPosition(lerp<sf::Vector2f>(position, newPosition, 0.01f));
	_view.setCenter(getPosition());
	if (maxDistance > distToZoom)
	{
		float s = maxDistance / distToZoom;
		_view.setSize(s * _viewSize);
	}	
	Camera::update(dt);
}