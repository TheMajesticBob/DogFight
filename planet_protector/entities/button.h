#pragma once
#include <ecm.h>
#include "../components/cmp_sprite.h"
#include "../components/cmp_text.h"
#include "GL/glew.h"
#include <SFML/Graphics.hpp>
#include "SFML/OpenGL.hpp"
#include "delegates.h"

DEFINE_DELEGATE(FButtonPtrDelegate, std::shared_ptr<class Button>);
DEFINE_DELEGATE(FButtonDelegate);

class Button : public Entity
{
public:
	Button() = delete;
	Button(Scene* const s, sf::Vector2f size);

	void update(double dt) override;
	void render() override;

	bool isMouseOver();

	void setDrawOnUi(bool drawOnUi);

	bool isActive() const;
	void setActive(bool _active);

	void setText(std::string text);
	void setSize(sf::Vector2f size);

	void setPressed(bool pressed) { _pressedState = pressed; }

	FButtonDelegate onButtonClicked;
	FButtonPtrDelegate onButtonPressed;
	FButtonPtrDelegate onButtonReleased;
	FButtonPtrDelegate onButtonHovered;
	FButtonPtrDelegate onButtonUnhovered;

protected:
	void HandleMouseClick();

	std::shared_ptr<ShapeComponent> _shapeComponent;
	std::shared_ptr<TextComponent> _textComponent;
	sf::Vector2f mp;
	bool _pressedState = false;
	bool _active = true;
	bool _hovered = false;
	

	sf::Color color;
};