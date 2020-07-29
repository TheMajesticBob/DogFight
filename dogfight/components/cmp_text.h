#pragma once
#include "cmp_sprite.h"
#include <SFML/Graphics/Text.hpp>
#include <ecm.h>

class TextComponent : public DrawableComponent {
public:
  TextComponent() = delete;
  explicit TextComponent(Entity* p, const std::string& str = "");
  void update(double dt) override;

 // void render() override;

  ~TextComponent() override = default;

  void SetText(const std::string& str);

  std::shared_ptr<sf::Text> getText() { return _text; }

protected:
  std::shared_ptr<sf::Font> _font;
  std::string _string;
  std::shared_ptr<sf::Text> _text;
};
