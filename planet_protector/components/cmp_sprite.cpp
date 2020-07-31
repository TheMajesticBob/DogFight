#include "system_ui.h"
#include "cmp_sprite.h"

using namespace std;


void SpriteComponent::setTexure(std::shared_ptr<sf::Texture> tex)
{
  _texture = tex;
  _sprite->setTexture(*_texture);
}

SpriteComponent::SpriteComponent(Entity* p)
    : DrawableComponent(p), _sprite(make_shared<sf::Sprite>())
{
	_drawable = std::make_shared<FDrawable>(_sprite);
}

void SpriteComponent::update(double dt) {
	_sprite->setPosition(_parent->getPosition());
	_sprite->setRotation(_parent->getRotation());
}

void DrawableComponent::render() { 
    if (_drawOnUI) {
        UI::queue(_drawable.get());
    }
    else {
        Renderer::queue(_drawable.get());
    }    
}

void ShapeComponent::update(double dt) {
  _shape->setPosition(_parent->getPosition());
  _shape->setRotation(-_parent->getRotation());
}

//void ShapeComponent::render() { Renderer::queue(_drawable.get()); }

sf::Shape& ShapeComponent::getShape() const { return *_shape; }

ShapeComponent::ShapeComponent(Entity* p)
    : DrawableComponent(p), _shape(make_shared<sf::CircleShape>())
{
	_drawable = std::make_shared<FDrawable>(_shape);
}

sf::Sprite& SpriteComponent::getSprite() const { return *static_cast<sf::Sprite*>(_drawable->drawable.get()); }
