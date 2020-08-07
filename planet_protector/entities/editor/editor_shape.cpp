#include "editor_shape.h"
#include "../../components/cmp_sprite.h"
#include "../../components/cmp_text.h"
#include "../button.h"

#include <system_renderer.h>
#include <input_handler.h>
#include <engine.h>
#include <utils.h>

#include <SFML/Graphics/CircleShape.hpp>

void ShapeEditor::EdgeText::render()
{
	if (shape && v1 >= 0 && v2 >= 0)
	{
		sf::Vector2f v1Pos = shape->getPoint(v1) - shape->getOrigin();
		sf::Vector2f v2Pos = shape->getPoint(v2) - shape->getOrigin();

		sf::Vector2f pos = (v1Pos + v2Pos) / 2.0f;
		t.setString(text);
		t.setFillColor(sf::Color::White);
		t.setPosition(pos);
		t.setOrigin(t.getLocalBounds().getSize() / 2.0f);
		t.setCharacterSize(36);
		t.setScale({ 0.2f,0.2f });
		Renderer::queue(&t);
	}
}


ShapeEditor::ShapeEditor(class Scene* const s)
	: Entity(s), _shapeComponent(addComponent<ShapeComponent>())
{
	_shapeComponent->setShape<sf::ConvexShape>();
	_shapeComponent->getShape().setPosition({ 0.0f, 0.0f });
	_shape = static_cast<sf::ConvexShape*>(&_shapeComponent->getShape());

	_textComponent = addComponent<TextComponent>();
	_textComponent->setDrawOnUI(true);

	ResetShape();
}

ShapeEditor::~ShapeEditor()
{
	_shapeComponent.reset();
}

void ShapeEditor::update(double dt)
{
	sf::Vector2f viewMousePos(sf::Mouse::getPosition(Engine::GetWindow()));
	sf::Vector2f worldMousePos(Utils::ScreenToWorld(viewMousePos));
	sf::RenderWindow& rw = Engine::GetWindow();
	const sf::View& mainView = rw.getView();

	float viewScale = mainView.getSize().x / rw.getSize().x;
	sf::Vector2f viewCenter = mainView.getCenter();
	sf::Vector2f screenCenter = (sf::Vector2f(rw.getSize()) / 2.0f);
	sf::Vector2f mouseToCenterOffset = worldMousePos - screenCenter;

	if (_textComponent)
	{
		std::string text = "Mouse Center Offset: " + to_string(mouseToCenterOffset.x) + ", " + to_string(mouseToCenterOffset.y) + "\n" +
			"View center: " + to_string(viewCenter.x) + ", " + to_string(viewCenter.y) + "\n" +
			"Screen Center: " + to_string(screenCenter.x) + ", " + to_string(screenCenter.y) + "\n" +
			"Scale: " + to_string(viewScale) + "\n" +
			"World mouse pos: " + to_string(worldMousePos.x) + ", " + to_string(worldMousePos.y);
		//_textComponent->SetPosition(worldMousePos);
		_textComponent->SetText(text);
	}

	if (InputHandler::GetMouseState(sf::Mouse::Right) && !InputHandler::GetLastMouseState(sf::Mouse::Right))
	{
		if (_currentHovered != nullptr)
		{
			int id = FindButtonId(_currentHovered);
			RemoveVertex(id);
			RegenerateVertexButtons();
		}
	}
	else if(_currentButton != nullptr)
	{
		sf::Vector2f mousePos = Utils::ScreenToWorld(sf::Vector2f(sf::Mouse::getPosition(Engine::GetWindow())));
		sf::Vector2f moveDelta = mousePos - _mouseStartPos;
		sf::Vector2f desiredPosition = _vertexStartPos + moveDelta;

		_shape->setPoint(_vertexId, desiredPosition);
	}
	else if(_currentHovered == nullptr && InputHandler::GetMouseState(sf::Mouse::Left) && !InputHandler::GetLastMouseState(sf::Mouse::Left))
	{
		PerformNewVertexCheck();
	}

	UpdateVertexButtonPositions();

	Entity::update(dt);
}

void ShapeEditor::PerformNewVertexCheck()
{
	sf::Vector2f mousePos = Utils::ScreenToWorld(sf::Vector2f(sf::Mouse::getPosition(Engine::GetWindow())));
	// Returns wrong position when zoomed in?
	for (int i = 0; i < _vertexButtons.size(); ++i)
	{
		int j = i + 1;
		if (j >= _vertexButtons.size())
			j = 0;

		sf::Vector2f b1Pos = _shape->getPoint(i) - _shape->getOrigin();
		sf::Vector2f b2Pos = _shape->getPoint(j) - _shape->getOrigin();

		float lengthS = lengthSquared(b2Pos - b1Pos);
		if (abs(lengthS) > 0.0001f)
		{
			float u = glm::clamp<float>(((mousePos.x - b1Pos.x)*(b2Pos.x - b1Pos.x) + (mousePos.y - b1Pos.y)*(b2Pos.y - b1Pos.y)) / lengthS, 0, 1);
			sf::Vector2f intersection(b1Pos + u * (b2Pos - b1Pos));
			float distanceFromLine = length(mousePos - intersection);

			if (i < _vertexButtons.size() - 1)
			{
				_edgeTexts[i].text = to_string(distanceFromLine);
			}

			if (distanceFromLine <= 1.0f)
			{
				AddVertexAt(j, mousePos + _shape->getOrigin());
				RegenerateVertexButtons();
				UpdateVertexButtonPositions();

				_currentButton = _vertexButtons[j];
				_currentButton->setPressed(true);
				_currentButton->setSize(sf::Vector2f(10.0f, 10.0f));
				_vertexId = j;

				_mouseStartPos = Utils::ScreenToWorld(sf::Vector2f(sf::Mouse::getPosition(Engine::GetWindow())));
				_vertexStartPos = _shape->getPoint(j);
				break;

			}
		}
	}
}

void ShapeEditor::render()
{
// 	for (auto& edge : _edgeTexts)
// 	{
// 		edge.render();
// 	}

	Entity::render();
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
	for (int i = _shape->getPointCount(); i < _vertexButtons.size(); ++i)
	{
		_vertexButtons[i]->setForDelete();
	}

	_vertexButtons.resize(_shape->getPointCount());
	_edgeTexts.resize(_shape->getPointCount() - 1);

	for (int i = 0; i < _shape->getPointCount(); ++i)
	{
		Button* button = _vertexButtons[i].get();
		if (!button)
		{
			_vertexButtons[i] = GetScene()->MakeEntity<Button>(sf::Vector2f(5.0f,5.0f));
			button = _vertexButtons[i].get();
			button->onButtonPressed += FButtonPtrDelegate::from_function<ShapeEditor, &ShapeEditor::OnButtonPressed>(this);
			button->onButtonReleased += FButtonPtrDelegate::from_function<ShapeEditor, &ShapeEditor::OnButtonReleased>(this);
			button->onButtonHovered += FButtonPtrDelegate::from_function<ShapeEditor, &ShapeEditor::OnButtonHovered>(this);
			button->onButtonUnhovered += FButtonPtrDelegate::from_function<ShapeEditor, &ShapeEditor::OnButtonUnhovered>(this);
		}

		if (i < _shape->getPointCount() - 1)
		{
			EdgeText& textComponent = _edgeTexts[i];
			textComponent.v1 = i;		// (i > 0 ? i : _shape->getPointCount()) - 1;
			textComponent.v2 = i + 1; // (i > 1 ? i : _shape->getPointCount()) - 2;
			textComponent.shape = _shape;
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

void ShapeEditor::AddVertexAt(int vertexId, sf::Vector2f pos)
{
	_shape->setPointCount(_shape->getPointCount() + 1);
	for (int i = _shape->getPointCount() - 1; i > vertexId; --i)
	{
		_shape->setPoint(i, _shape->getPoint(i - 1));
	}
	_shape->setPoint(vertexId, pos);
}

void ShapeEditor::RemoveVertex(int vertexId)
{
	for (int i = vertexId; i < _shape->getPointCount() - 1; ++i)
	{
		_shape->setPoint(i, _shape->getPoint(i + 1));
	}
	_shape->setPointCount(_shape->getPointCount() - 1);
}

void ShapeEditor::OnButtonHovered(std::shared_ptr<class Button> button)
{
	_currentHovered = button;
}

void ShapeEditor::OnButtonUnhovered(std::shared_ptr<class Button> button)
{
	_currentHovered = nullptr;
}

void ShapeEditor::OnButtonPressed(std::shared_ptr<class Button> button)
{
	if (!_currentButton)
	{
		_currentButton = button;
		_currentButton->setSize(sf::Vector2f(10.0f, 10.0f));
		_vertexId = FindButtonId(button);

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

int ShapeEditor::FindButtonId(std::shared_ptr<class Button> button)
{
	for (int i = 0; i < _vertexButtons.size(); ++i)
	{
		if (_vertexButtons[i] == button)
		{
			return i;
		}
	}

	return -1;
}
