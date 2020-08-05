#include "scene_editor.h"
#include "../planet_protector/game.h"
#include "../entities/editor/editor_shape.h"
#include "../entities/camera.h"

#include <SFML/Window/Mouse.hpp>

#include <input_handler.h>
#include <utils.h>

void EditorScene::Load()
{
	_shapeEditor = MakeEntity<ShapeEditor>();
	_shapeEditor->setPosition({ 0.0f,0.0f });

	_camera = MakeEntity<Camera>();
	_camera->setPosition({ 0.0f, 0.0f });
	_camera->SetLerpPosition(true);
	_camera->SetLerpFactor(0.1f);

	InputHandler::BindKey(sf::Keyboard::Escape, sf::Event::KeyPressed, FKeyDelegate::from_function<EditorScene, &EditorScene::Exit>(this));
	
	Scene::Load();
}

void EditorScene::UnLoad()
{
	InputHandler::UnbindKey(sf::Keyboard::Escape, sf::Event::KeyPressed, FKeyDelegate::from_function<EditorScene, &EditorScene::Exit>(this));

	_shapeEditor.reset();

	Scene::UnLoad();
}

void EditorScene::Update(const double& dt)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		if (!_isMovePressed)
		{
			MovePressed();
			_moveStartPos = sf::Vector2f(sf::Mouse::getPosition(Engine::GetWindow()));
			_cameraStartPos = _camera->getPosition();
		}
	}
	else
	{
		MoveReleased();
	}

	if (_isMovePressed)
	{
		sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(Engine::GetWindow()));
		sf::Vector2f moveDelta = _moveStartPos - mousePos;
		sf::Vector2f desiredPos = _cameraStartPos + moveDelta;
		_camera->SetDesiredPosition(desiredPos);
	}

	Scene::Update(dt);
}

void EditorScene::Exit()
{
	Engine::ChangeScene(&menu);
}
