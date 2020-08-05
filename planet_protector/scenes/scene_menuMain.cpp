#include "scene_menuMain.h"
#include "../game.h"
#include "../entities/planet.h"
#include "system_resources.h"
#include "../components/cmp_text.h"
#include <SFML/Graphics.hpp>
#include "../entities/camera.h"
#include "../entities/button.h"
#include "../entities/ambient_manager.h"
#include <input_handler.h>

using namespace sf;
using namespace std;

void MainMenu::MoveToGame()
{
	Engine::ChangeScene(&testing);
}

void MainMenu::MoveToSettings()
{
	Engine::ChangeScene((Scene*)&settings);
}

void MainMenu::Load()
{
	//create camera view
	auto camera = MakeEntity<Camera>();
	//camera->setPosition(Vector2f(1000.0f, 500.0f));
	camera->setPosition(Vector2f(0.0f, 0.0f));
	camera->setScale(1.0f);
	
	// Create planet
	float planetRadius = Engine::GetWindowSize().x / 2;
	float planetMass = 1000000.0f;
	auto planet = MakeEntity<Planet>(planetRadius, planetMass);

	// Create Button
	Vector2f buttonSize(400.0f, 75.0f);

	// Menu Header
	_head = MakeEntity<Button>(Vector2f(500.0f, 60.0f));
	_head->setText("~Planet Protector~");
	_head->setPosition(Vector2f(Engine::GetWindowSize().x / 2, Engine::GetWindowSize().y / 2 - 100));
	// Stops header from being highlighted
	_head->setActive(false); 
	   
	// Play Button
	_play = MakeEntity<Button>(buttonSize);
	_play->onButtonClicked = FButtonDelegate::from_function<MainMenu, &MainMenu::MoveToGame>(this);
	_play->setPosition(Vector2f(Engine::GetWindowSize().x / 2, Engine::GetWindowSize().y/2));
	_play->setText("Play");

	if (ents.find("AmbientManager").size() <= 0)
	{
		_ambientManager = MakeEntity<AmbientManager>();
		_ambientManager->addTag("AmbientManager");
		_ambientManager->loadLayer("menu_ambient", "menu_ambient.wav");
		_ambientManager->play("menu_ambient");
		_ambientManager->setPersistent(true);
	}

	// Settings Button
	_settings = MakeEntity<Button>(buttonSize);
	//_settings->onButtonClicked = FButtonClicked::from_function<MainMenu, &MainMenu::MoveToSettings>(this);
	_settings->setPosition(Vector2f(Engine::GetWindowSize().x / 2, (Engine::GetWindowSize().y / 2) + 100));
	_settings->setText("Settings");		

	InputHandler::BindKey(sf::Keyboard::Escape, sf::Event::KeyPressed, FKeyDelegate::from_function<MainMenu, &MainMenu::Quit>(this));

	Scene::Load();
}

void MainMenu::UnLoad()
{
	InputHandler::UnbindKey(sf::Keyboard::Escape, sf::Event::KeyPressed, FKeyDelegate::from_function<MainMenu, &MainMenu::Quit>(this));

	_play.reset();
	_settings.reset();

	Scene::UnLoad();
}

void MainMenu::Quit()
{
	Engine::GetWindow().close();
}

void MainMenu::Update(const double& dt)
{	
	if (Keyboard::isKeyPressed(Keyboard::Num1))
	{
		Engine::ChangeScene(&editorScene);
	}

	Scene::Update(dt);
}
