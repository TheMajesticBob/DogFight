#include "scene_menuMain.h"
#include "../game.h"
#include "../entities/planet.h"
#include "system_resources.h"
#include "../components/cmp_text.h"
#include <SFML/Graphics.hpp>
#include "../entities/camera.h"
#include "../entities/button.h"
#include "../entities/ambient_manager.h"

using namespace sf;
using namespace std;

void MainMenu::MoveToGame()
{
	Engine::ChangeScene(&testing, true);
}

void MainMenu::MoveToSettings()
{
	Engine::ChangeScene((Scene*)&settings);
}

void MainMenu::Load()
{
	//create camera view
	auto camera = makeEntity<Camera>();
	//camera->setPosition(Vector2f(1000.0f, 500.0f));
	camera->setPosition(Vector2f(0.0f, 0.0f));
	camera->setScale(1.0f);
	
	// Create planet
	float planetRadius = Engine::getWindowSize().x / 2;
	float planetMass = 1000000.0f;
	auto planet = makeEntity<Planet>(planetRadius, planetMass);	

	// Create Button
	Vector2f buttonSize(400.0f, 75.0f);

	// Menu Header
	_head = makeEntity<Button>(Vector2f(500.0f, 60.0f));
	_head->setText("~Planet Protector~");
	_head->setPosition(Vector2f(Engine::getWindowSize().x / 2, Engine::getWindowSize().y / 2 - 100));
	// Stops header from being highlighted
	_head->setActive(false); 
	   
	// Play Button
	_play = makeEntity<Button>(buttonSize);
	_play->onButtonClicked = FButtonClicked::from_function<MainMenu, &MainMenu::MoveToGame>(this);
	_play->setPosition(Vector2f(Engine::getWindowSize().x / 2, Engine::getWindowSize().y/2));
	_play->setText("Play");

	if (ents.find("AmbientManager").size() <= 0)
	{
		_ambientManager = makeEntity<AmbientManager>();
		_ambientManager->addTag("AmbientManager");
		_ambientManager->loadLayer("menu_ambient", "menu_ambient.wav");
		_ambientManager->play("menu_ambient");
		_ambientManager->setPersistent(true);
	}

	// Settings Button
	_settings = makeEntity<Button>(buttonSize);
	//_settings->onButtonClicked = FButtonClicked::from_function<MainMenu, &MainMenu::MoveToSettings>(this);
	_settings->setPosition(Vector2f(Engine::getWindowSize().x / 2, (Engine::getWindowSize().y / 2) + 100));
	_settings->setText("Settings");		

	setLoaded(true);
}

void MainMenu::UnLoad()
{
	Scene::UnLoad();

	_play.reset();
	_settings.reset();
}

void MainMenu::Update(const double& dt)
{	
	if (Keyboard::isKeyPressed(Keyboard::Num1))
	{
		Engine::ChangeScene(&testing, true);
	}

	Scene::Update(dt);
}
