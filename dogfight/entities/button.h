#pragma once
#include <ecm.h>
#include "../components/cmp_sprite.h"
#include "../components/cmp_text.h"
#include "GL/glew.h"
#include <SFML/Graphics.hpp>
#include "SFML/OpenGL.hpp"

class Button : public Entity
{
public:
	Button() = delete;
	Button(Scene* const s, sf::Vector2f size);

	void update(double dt) override;
	void render() override;

	bool isMouseOver();
	bool MouseClick();

	void setText(std::string text);

protected:	

	std::shared_ptr<ShapeComponent> shapeComponent;
	std::shared_ptr<TextComponent> textComponent;
	sf::Vector2f mp;
	bool pressedState = false;

	sf::Color color;
};