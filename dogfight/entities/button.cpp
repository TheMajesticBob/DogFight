#include "button.h"
#include <engine.h>
#include "../engine/game_resources.h"

using namespace sf;

Button::Button(Scene* const s, Vector2f size) : Entity(s)
{

	shapeComponent = addComponent<ShapeComponent>();
	shapeComponent->setShape<RectangleShape>(Vector2f(size.x, size.y));
	shapeComponent->getShape().setOutlineColor(Color::Magenta);
	shapeComponent->getShape().setOutlineColor(Color::Magenta);
	shapeComponent->getShape().setOrigin(size.x / 2, size.y / 2);
}

void Button::update(double dt)
{
	Event event;

	Entity::update(dt);
}

void Button::render()
{
	Entity::render();
}

bool Button::isMouseOver()
{
	//if (){}
	return false;
}

bool Button::MouseClick()
{
	return false;
}
