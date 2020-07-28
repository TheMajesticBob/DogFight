#pragma once
#include <ecm.h>
#include <SFML/Graphics.hpp>

class Camera : public Entity
{
	protected:
		std::vector<std::shared_ptr<Entity>> _entitiesToFollow;
		sf::View _view;
		sf::Vector2f _viewSize;

		float distToZoom = 500.0f;

	public:
		Camera() = delete;
		Camera(Scene* const s);

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