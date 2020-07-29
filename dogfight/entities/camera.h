#pragma once
#include <ecm.h>
#include <SFML/Graphics.hpp>

struct FollowedEntity
{
	FollowedEntity(std::shared_ptr<Entity> entity, float weight)
		: _Entity(entity), _Weight(weight)
	{

	}

	std::shared_ptr<Entity> getEntity() { return _Entity; }
	float getWeight() { return _Weight; }

private:
	std::shared_ptr<Entity> _Entity;
	float _Weight;
};

class Camera : public Entity
{
	protected:
		std::vector<FollowedEntity> _entitiesToFollow;
		sf::View _view;
		sf::Vector2f _viewSize;

		float distToZoom = 1000.0f;

	public:
		Camera() = delete;
		Camera(Scene* const s);

		void AddFollow(std::shared_ptr<Entity> e, float weight = 1.0f)
		{
			_entitiesToFollow.push_back(FollowedEntity(e, weight));
		}

		void RemoveFollow(std::shared_ptr<Entity> e)
		{
			// TODO: Find the right struct and remove it based on passed pointer
			// _entitiesToFollow.erase(std::remove(_entitiesToFollow.begin(), _entitiesToFollow.end(), e), _entitiesToFollow.end());
		}

		void update(double) override;
};