#include "button.h"
#include <engine.h>
#include "../engine/game_resources.h"
#include <input_handler.h>

using namespace sf;

Button::Button(Scene* const s, Vector2f size) : Entity(s)
{
	textComponent = addComponent<TextComponent>();
	textComponent->setDrawOnUI(true);
	textComponent->setLayer(1);
	shapeComponent = addComponent<ShapeComponent>();
	shapeComponent->setDrawOnUI(true);
	setSize(size);
}

void Button::update(double dt)
{
	//find location of mouse in window	
	mp.x = Mouse::getPosition(Engine::GetWindow()).x;
	mp.y = Mouse::getPosition(Engine::GetWindow()).y;
		
	//mouse over change colour of buton to a dark grey
	if (isActive()) 
	{
		if (pressedState)
		{
			shapeComponent->getShape().setFillColor(Color::Color(50, 15, 70, 255));
		}
		else 
		{
			if (isMouseOver())
			{
				if (!_hovered)
				{
					onButtonHovered.invokeSafe(getShared<Button>());
					_hovered = true;
				}
				shapeComponent->getShape().setFillColor(Color::Color(100, 30, 140, 255));
			}
			else
			{
				if (_hovered)
				{
					onButtonUnhovered.invokeSafe(getShared<Button>());
					_hovered = false;
				}
				shapeComponent->getShape().setFillColor(Color::Black);
			}
		}
	}	


	HandleMouseClick();

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

void Button::setDrawOnUi(bool drawOnUi)
{
	shapeComponent->setDrawOnUI(drawOnUi);
	textComponent->setDrawOnUI(drawOnUi);
}

void Button::HandleMouseClick()
{
	Event _event;
	sf::RenderWindow& window = Engine::GetWindow();
	
	if (!pressedState)
	{
		if (InputHandler::GetMouseState(Mouse::Button::Left) && isMouseOver())
		{
			if (!InputHandler::GetLastMouseState(Mouse::Button::Left))
			{
				pressedState = true;
				onButtonPressed.invokeSafe(std::static_pointer_cast<Button>(getShared<Button>()));
			}
		}
	}
	if (pressedState && !InputHandler::GetMouseState(Mouse::Button::Left))
	{
		pressedState = false;
		onButtonReleased.invokeSafe(std::static_pointer_cast<Button>(getShared<Button>()));

		if (isMouseOver())
		{
			onButtonClicked.invokeSafe();
		}
	}	
}

bool Button::isActive() const { return _active; }

void Button::setActive(bool _active) { Button::_active = _active; }

void Button::setText(std::string text)
{	
	textComponent->SetText(text);
	textComponent->getText()->setOrigin(textComponent->getText()->getLocalBounds().getSize()/2.0f);
}

void Button::setSize(sf::Vector2f size)
{
	shapeComponent->setShape<RectangleShape>(Vector2f(size.x, size.y));
	shapeComponent->getShape().setFillColor(Color::Black);
	shapeComponent->getShape().setOutlineColor(Color::Magenta);
	shapeComponent->getShape().setOutlineThickness(2);
	shapeComponent->getShape().setOrigin(size.x / 2, size.y / 2);
}
