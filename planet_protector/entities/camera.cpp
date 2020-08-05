#include "camera.h"
#include "engine.h"
#include <system_renderer.h>

void Camera::setPositionInternal(sf::Vector2f position)
{
	Entity::setPosition(position);
	_view.setCenter(position);
}

Camera::Camera(Scene* const s)
	: Entity(s), _shouldLerpPosition(true), _positionLerpFactor(0.01f),
	_shouldLerpScale(false), _scaleLerpFactor(0.1f), _desiredScale(1.0f)
{
	_view = sf::View(sf::FloatRect(0.0f, 0.0f, Engine::GetWindowSize().x, Engine::GetWindowSize().y));
	_viewSize = _view.getSize();
}

Camera::~Camera()
{

}

void Camera::update(double)
{
	Engine::GetWindow().setView(_view);

	if (_shouldLerpPosition)
	{
		setPositionInternal(lerp<sf::Vector2f>(getPosition(), _desiredPosition, _positionLerpFactor));
	}

	if (_shouldLerpScale)
	{
		float currentScale = _view.getSize().x / _viewSize.x;
		_view.setSize(lerp<float>(currentScale, _desiredScale, _scaleLerpFactor) * _viewSize);
	}
}

void Camera::setPosition(sf::Vector2f position)
{
	_desiredPosition = position;
	setPositionInternal(position);
}

void Camera::setScale(float scale)
{
	_view.setSize(scale * _viewSize);
}

FollowCamera::FollowCamera(Scene* const s) : Camera(s)
{
	_shouldLerpPosition = true;
	_shouldLerpScale = true;
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
	_desiredPosition = newPosition;

	// Interpolate between old and new
	_view.setCenter(getPosition());
	if (maxDistance > distToZoom)
	{
		float s = maxDistance / distToZoom;
		SetDesiredScale(s);
	}	
	Camera::update(dt);
}