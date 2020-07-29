#include "button.h"
#include <engine.h>
#include "../engine/game_resources.h"

using namespace sf;

Button::Button(Scene* const s, Vector2f size) : Entity(s)
{
	shapeComponent = addComponent<ShapeComponent>();
	shapeComponent->setShape<RectangleShape>(Vector2f(size.x, size.y));
	shapeComponent->getShape().setOutlineColor(Color::Magenta);
	shapeComponent->getShape().setOutlineThickness(2);
	shapeComponent->getShape().setOrigin(0,0);

	//event = Mouse::Left;

	

	//make outline bigger and fill colour black
}

void Button::update(double dt)
{
	//find location of mouse in window	
	mp.x = Mouse::getPosition(Engine::GetWindow()).x;
	mp.y = Mouse::getPosition(Engine::GetWindow()).y;
		
	//mouse over change colour of buton to a dark grey
	if (isMouseOver()) {
		shapeComponent->getShape().setFillColor(Color::Green);
	}
	else {
		shapeComponent->getShape().setFillColor(Color::Black);
	}
	Entity::update(dt);
}

void Button::render()
{
	Entity::render();
}

bool Button::isMouseOver()
{	
	auto bounds = shapeComponent->getShape().getGlobalBounds();//.contains(mp);
	if (bounds.contains(mp)) {
		return true;
	}
	return false;
		//return bounds.contains(mp);
}

bool Button::MouseClick()
{
	
	//if (isMouseOver() && Event::MouseButtonReleased /*&& event == Mouse::Left*/) {
	//	return true;
	//}
	//else {
	//	return false;
	//}
	return false;
	
}
