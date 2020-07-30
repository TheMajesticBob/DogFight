#pragma once

#include "ecm.h"
#include "system_renderer.h"
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Sprite.hpp>

class DrawableComponent : public Component
{
protected:
	std::shared_ptr<FDrawable> _drawable;
    bool _drawOnUI = false;

public:
	DrawableComponent() = delete;
	explicit DrawableComponent(Entity* p) : Component(p) {}
    void render() override;
	void setLayer(int layer) { _drawable->layer = layer; }
    void setDrawOnUI(bool drawOnUI) { _drawOnUI = drawOnUI; }
};

class SpriteComponent : public DrawableComponent {
protected:
  std::shared_ptr<sf::Sprite> _sprite;
  std::shared_ptr<sf::Texture> _texture;

public:
  SpriteComponent() = delete;

  explicit SpriteComponent(Entity* p);
  void update(double dt) override;
 

  void setTexure(std::shared_ptr<sf::Texture> tex);

  sf::Sprite& getSprite() const;
};

class ShapeComponent : public DrawableComponent {
protected:
  std::shared_ptr<sf::Shape> _shape;

public:
  ShapeComponent() = delete;

  explicit ShapeComponent(Entity* p);

  void update(double dt) override;
  

  sf::Shape& getShape() const;

  template <typename T>
  void setShapeExplicit(std::shared_ptr<T> shape)
  {
	  _shape = shape;
	  _drawable->drawable = _shape;
  }

  template <typename T>
  void setShape(std::shared_ptr<T> shape)
  {
	  _shape = shape;
	  _drawable->drawable = _shape;
  }

  template <typename T, typename... Targs>
  void setShape(Targs... params) 
  {
    _shape.reset(new T(params...));
	_drawable->drawable = _shape;
  }
};