#include "editor_shape.h"
#include "../../components/cmp_sprite.h"
#include "../button.h"

#include <engine.h>
#include <utils.h>

#include <SFML/Graphics/CircleShape.hpp>

ShapeEditor::ShapeEditor(class Scene* const s)
	: Entity(s), _shapeComponent(addComponent<ShapeComponent>())
{
	_shapeComponent->setShape<sf::ConvexShape>();
	_shapeComponent->getShape().setPosition({ 0.0f, 0.0f });
	_shape = static_cast<sf::ConvexShape*>(&_shapeComponent->getShape());

	ResetShape();
}

ShapeEditor::~ShapeEditor()
{
	_shapeComponent.reset();
}

void ShapeEditor::update(double dt)
{
	if(_currentButton != nullptr)
	{
		sf::Vector2f mousePos = Utils::ScreenToWorld(sf::Vector2f(sf::Mouse::getPosition(Engine::GetWindow())));
		sf::Vector2f moveDelta = mousePos - _mouseStartPos;
		sf::Vector2f desiredPosition = _vertexStartPos + moveDelta;

		_shape->setPoint(_vertexId, desiredPosition);
		// _currentButton->setPosition();
	}

	UpdateVertexButtonPositions();

	Entity::update(dt);
}

void ShapeEditor::ResetShape()
{
	if (_shapeComponent)
	{
		sf::CircleShape circle(100.0f);

		_shape->setPointCount(circle.getPointCount());
		for (int i = 0; i < _shape->getPointCount(); ++i)
		{
			_shape->setPoint(i, circle.getPoint(i));
		}

		_shape->setOrigin({ 100.0f, 100.0f });
		_shape->setFillColor(sf::Color::Transparent);
		_shape->setOutlineColor(sf::Color::White);
		_shape->setOutlineThickness(2.0f);

		RegenerateVertexButtons();
	}
}

void ShapeEditor::RegenerateVertexButtons()
{
	_vertexButtons.resize(_shape->getPointCount());

	Button* button = nullptr;

	for (int i = 0; i < _shape->getPointCount(); ++i)
	{
		button = _vertexButtons[i].get();
		if (!button)
		{
			_vertexButtons[i] = GetScene()->MakeEntity<Button>(sf::Vector2f(5.0f,5.0f));
			button = _vertexButtons[i].get();
			button->onButtonPressed += FButtonPtrDelegate::from_function<ShapeEditor, &ShapeEditor::OnButtonPressed>(this);
			button->onButtonReleased += FButtonPtrDelegate::from_function<ShapeEditor, &ShapeEditor::OnButtonReleased>(this);
		}
	}
}

void ShapeEditor::UpdateVertexButtonPositions()
{
	for (int i = 0; i < _vertexButtons.size(); ++i)
	{
		Button* button = _vertexButtons[i].get();
		if (button)
		{
			sf::Vector2f screenPosition = Utils::WorldToScreen(_shape->getPosition() - _shape->getOrigin() + _shape->getPoint(i));
			button->setPosition(screenPosition);
		}
	}
}

void ShapeEditor::OnButtonPressed(std::shared_ptr<class Button> button)
{
	if (!_currentButton)
	{
		_currentButton = button;
		_currentButton->setSize(sf::Vector2f(10.0f, 10.0f));
		for (int i = 0; i < _vertexButtons.size(); ++i)
		{
			if (_vertexButtons[i] == button)
			{
				_vertexId = i;
				break;
			}
		}


		_mouseStartPos = Utils::ScreenToWorld(sf::Vector2f(sf::Mouse::getPosition(Engine::GetWindow())));
		_vertexStartPos = _shape->getPoint(_vertexId);
	}
}

void ShapeEditor::OnButtonReleased(std::shared_ptr<class Button> button)
{
	_currentButton->setSize(sf::Vector2f(5.0f, 5.0f));

	_currentButton = nullptr; 
	_vertexId = -1;
}
