#include "button.h"
#include <engine.h>
#include "../engine/game_resources.h"

using namespace sf;

Button::Button(Scene* const s, Vector2f size) : Entity(s)
{
	textComponent = addComponent<TextComponent>();
	textComponent->setDrawOnUI(true);
	textComponent->setLayer(1);
	shapeComponent = addComponent<ShapeComponent>();
	shapeComponent->setShape<RectangleShape>(Vector2f(size.x, size.y));
	shapeComponent->getShape().setFillColor(Color::Black);
	shapeComponent->getShape().setOutlineColor(Color::Magenta);
	shapeComponent->getShape().setOutlineThickness(2);
	shapeComponent->getShape().setOrigin(size.x/2,size.y/2); 
	shapeComponent->setDrawOnUI(true);	
}

void Button::update(double dt)
{
	//find location of mouse in window	
	mp.x = Mouse::getPosition(Engine::GetWindow()).x;
	mp.y = Mouse::getPosition(Engine::GetWindow()).y;
		
	//mouse over change colour of buton to a dark grey
	if (isActive()) {
		if (isMouseOver()) {
			shapeComponent->getShape().setFillColor(Color::Color(100,30,140,255));
		}
		else {
			shapeComponent->getShape().setFillColor(Color::Black);
		}
		if (MouseClick()) {
			onButtonClicked.invokeSafe();
			return;
			std::cout << "Success" << std::endl;
		}
	}	

	Entity::update(dt);
}

void Button::render()
{
	Entity::render();
}


bool Button::isMouseOver()
{
	if (!shapeComponent->is_fordeletion())
	{
		auto bounds = shapeComponent->getShape().getGlobalBounds();
		if (bounds.contains(mp))
		{
			return true;
		}
	}
	return false;
}

bool Button::MouseClick()
{
	Event _event;
	sf::RenderWindow& window = Engine::GetWindow();
		
	if (isMouseOver()) {
		if (Mouse::isButtonPressed(Mouse::Button::Left)) {
			pressedState = true;
		}
		else if (pressedState == true) {
			pressedState = false;
			return true;
		}
	}
	else {
		if (!Mouse::isButtonPressed(Mouse::Button::Left)) {
			pressedState = false;
		}
	}

		return false;		
}

bool Button::isActive() const { return _active; }

void Button::setActive(bool _active) { Button::_active = _active; }

void Button::setText(std::string text)
{	
	textComponent->SetText(text);
	textComponent->getText()->setOrigin(textComponent->getText()->getLocalBounds().getSize()/2.0f);
}
