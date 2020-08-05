#pragma once
#include "maths.h"
#include <SFML/Graphics/Transform.hpp>
#include <algorithm>
#include <memory>
#include <set>
#include <typeindex>
#include <vector>

class Entity;
class Scene;

class Component {
  friend Entity;

protected:

	Entity* const _parent;
	Component* _parentComponent;

	sf::Transform _transform;

	bool _fordeletion; // should be removed
	bool _isVisible;
	explicit Component(Entity* const p);

public:
	Component() = delete;

	bool is_fordeletion() const;
	bool is_visible() const { return _isVisible; }

	sf::Transform getLocalTransform() { return _transform; }
	
	sf::Transform getTransform() 
	{
		if (_parentComponent) 
		{
			return _transform * _parentComponent->getTransform();
		}

		return _transform;
	}

  
	void setVisibility(bool visible) { _isVisible = visible; }

	virtual void onBeginPlay() {}

	virtual void update(double dt) = 0;

	virtual void render() = 0;

	virtual ~Component();
};


class Entity {
	friend struct EntityManager;
public:
	enum Team
	{
		T_PLAYER,
		T_ENEMY,
		T_TEAM3,
		T_TEAM4
	};
private:
	std::shared_ptr<Entity> _shared_to_me;

protected:
  std::vector<std::shared_ptr<Component>> _components;
  sf::Vector2f _position;
  float _rotation;
  bool _alive;       // should be updated
  bool _visible;     // should be rendered
  bool _fordeletion; // should be deleted
  bool _persistent = false;  // should persist between scenes
  std::set<std::string> _tags;

  Scene* const _scene;
  Team _team;

public:
	void SetTeam(Team newTeam) { _team = newTeam; }
	const Team& GetTeam() { return _team; }

	std::shared_ptr<Entity> getShared() { return _shared_to_me; }
	void setSharedPtr(std::shared_ptr<Entity> ptr) { _shared_to_me = ptr; }

  void addTag(const std::string& t);
  const std::set<std::string>& getTags() const;
  Entity(Scene* const s);

  virtual ~Entity();

  virtual void update(double dt);

  virtual void render();

  virtual void OnBeginOverlap(Entity* const e) {}
  virtual void OnEndOverlap(Entity* const e) {}

  virtual void OnHit(float damage) {}

  //
  const sf::Vector2f& getPosition() const;
  Scene* const GetScene() { return _scene; }

  void setPosition(const sf::Vector2f& _position);

  bool is_fordeletion() const;

  float getRotation() const;

  void setRotation(float _rotation);

  bool isAlive() const;

  void setAlive(bool _alive);

  bool is_persistent() { return _persistent; }
  void setPersistent(bool persistent) { _persistent = persistent; }

  void setForDelete();

  bool isVisible() const;

  void setVisible(bool _visible);

  template <typename T, typename... Targs>
  std::shared_ptr<T> addComponent(Targs... params) {
    static_assert(std::is_base_of<Component, T>::value, "T != component");
    std::shared_ptr<T> sp(std::make_shared<T>(this, params...));
    _components.push_back(sp);
    return sp;
  }

  template <typename T>
  const std::vector<std::shared_ptr<T>> get_components() const {
    static_assert(std::is_base_of<Component, T>::value, "T != component");
    std::vector<std::shared_ptr<T>> ret;
    for (const auto c : _components) {
      if (typeid(*c) == typeid(T)) {
        ret.push_back(std::dynamic_pointer_cast<T>(c));
      }
    }
    return std::move(ret);
  }

  // Will return a T component, or anything derived from a T component.
  template <typename T>
  const std::vector<std::shared_ptr<T>> GetCompatibleComponent() {
    static_assert(std::is_base_of<Component, T>::value, "T != component");
    std::vector<std::shared_ptr<T>> ret;
    for (auto c : _components) {
      auto dd = dynamic_cast<T*>(&(*c));
      if (dd) {
        ret.push_back(std::dynamic_pointer_cast<T>(c));
      }
    }
    return ret;
  }
};

struct EntityManager {
	std::vector<std::shared_ptr<Entity>> list;
	void update(double dt);
	void render();
	std::vector<std::shared_ptr<Entity>> find(const std::string& tag) const;
	std::vector<std::shared_ptr<Entity>>
		find(const std::vector<std::string>& tags) const;
	void clear() {
		for (std::shared_ptr<Entity> e : list)
		{
			e->setForDelete();
		}
		list.clear();
	}
};

class Pawn : public Entity
{
    // base class for a controllable entity with a pointer to its controller
protected:
    //Controller
public:
    Pawn() = delete;
    Pawn(Scene* const s);

    void update(double dt) override;
    class Controller *ptrControl;
};
