#pragma once
#include <ecm.h>
#include "../components/cmp_sprite.h"
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

protected:	

	std::shared_ptr<ShapeComponent> shapeComponent;

	GLfloat drawVerts[8];
	sf::Color color;
};