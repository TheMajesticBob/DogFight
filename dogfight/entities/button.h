#pragma once
#include <ecm.h>
#include "../components/cmp_sprite.h"
#include "../components/cmp_text.h"
#include "GL/glew.h"
#include <SFML/Graphics.hpp>
#include "SFML/OpenGL.hpp"
#include "delegates.h"

DEFINE_DELEGATE(FButtonClicked);

class Button : public Entity
{
public:
	Button() = delete;
	Button(Scene* const s, sf::Vector2f size);

	void update(double dt) override;
	void render() override;

	bool isMouseOver();
	bool MouseClick();

	bool isActive() const;
	void setActive(bool _active);

	void setText(std::string text);

	FButtonClicked onButtonClicked;

protected:	

	std::shared_ptr<ShapeComponent> shapeComponent;
	std::shared_ptr<TextComponent> textComponent;
	sf::Vector2f mp;
	bool pressedState = false;
	bool _active = true;
	

	sf::Color color;
};