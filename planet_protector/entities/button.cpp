#include "button.h"
#include <engine.h>
#include "../engine/game_resources.h"
#include <input_handler.h>

using namespace sf;

Button::Button(Scene* const s, Vector2f size) : Entity(s)
{
	_textComponent = addComponent<TextComponent>();
	_textComponent->setDrawOnUI(true);
	_textComponent->setLayer(1);
	_shapeComponent = addComponent<ShapeComponent>();
	_shapeComponent->setDrawOnUI(true);
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
		if (_pressedState)
		{
			_shapeComponent->getShape().setFillColor(Color::Color(50, 15, 70, 255));
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
				_shapeComponent->getShape().setFillColor(Color::Color(100, 30, 140, 255));
			}
			else
			{
				if (_hovered)
				{
					onButtonUnhovered.invokeSafe(getShared<Button>());
					_hovered = false;
				}
				_shapeComponent->getShape().setFillColor(Color::Black);
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
	if (!_shapeComponent->is_fordeletion())
	{
		auto bounds = _shapeComponent->getShape().getGlobalBounds();
		if (bounds.contains(mp))
		{
			return true;
		}
	}
	return false;
}

void Button::setDrawOnUi(bool drawOnUi)
{
	_shapeComponent->setDrawOnUI(drawOnUi);
	_textComponent->setDrawOnUI(drawOnUi);
}

void Button::HandleMouseClick()
{
	Event _event;
	sf::RenderWindow& window = Engine::GetWindow();
	
	if (!_pressedState)
	{
		if (InputHandler::GetMouseState(Mouse::Button::Left) && isMouseOver())
		{
			if (!InputHandler::GetLastMouseState(Mouse::Button::Left))
			{
				_pressedState = true;
				onButtonPressed.invokeSafe(std::static_pointer_cast<Button>(getShared<Button>()));
			}
		}
	}
	if (_pressedState && !InputHandler::GetMouseState(Mouse::Button::Left))
	{
		_pressedState = false;
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
	_textComponent->SetText(text);
	_textComponent->getText()->setOrigin(_textComponent->getText()->getLocalBounds().getSize()/2.0f);
}

void Button::setSize(sf::Vector2f size)
{
	_shapeComponent->setShape<RectangleShape>(Vector2f(size.x, size.y));
	_shapeComponent->getShape().setFillColor(Color::Black);
	_shapeComponent->getShape().setOutlineColor(Color::Magenta);
	_shapeComponent->getShape().setOutlineThickness(2);
	_shapeComponent->getShape().setOrigin(size.x / 2, size.y / 2);
}
