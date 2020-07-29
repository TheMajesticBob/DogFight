#pragma once
#include <ecm.h>
#include <SFML/Graphics.hpp>

class Camera :public Entity 
{
protected:
	sf::View _view;
	sf::Vector2f _viewSize;

public:
	Camera() = delete;
	Camera(Scene* const s);

	void update(double) override;

	void setPosition(sf::Vector2f position);
	void setScale(float scale);

};

class FollowCamera : public Camera
{
	protected:
		std::vector<std::shared_ptr<Entity>> _entitiesToFollow;

		float distToZoom = 500.0f;

	public:
		FollowCamera() = delete;
		FollowCamera(Scene* const s);

		void AddFollow(std::shared_ptr<Entity> e)
		{
			_entitiesToFollow.push_back(e);
		}

		void RemoveFollow(std::shared_ptr<Entity> e)
		{
			_entitiesToFollow.erase(std::remove(_entitiesToFollow.begin(), _entitiesToFollow.end(), e), _entitiesToFollow.end());
		}

		void update(double) override;
};