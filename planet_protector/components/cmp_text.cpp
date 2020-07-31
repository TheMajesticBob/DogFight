#include "cmp_text.h"
#include <system_renderer.h>
#include <system_resources.h>

void TextComponent::update(double dt) {
    _text->setPosition(_parent->getPosition());
    _text->setRotation(_parent->getRotation());
}

//void TextComponent::render() { Renderer::queue(&_text); }

TextComponent::TextComponent(Entity* const p, const std::string& str) 
    : DrawableComponent(p), _text(std::make_shared<sf::Text>()), _string(str) {
  _text->setString(_string);
  _font = Resources::get<sf::Font>("RobotoMono-Regular.ttf");
  _text->setFont(*_font);
  _drawable = std::make_shared<FDrawable>(_text);
}

void TextComponent::SetText(const std::string& str) {
  _string = str;
  _text->setString(_string);
}
