#include "scene_menuSettings.h"
#include "../game.h"
#include "system_resources.h"
#include "../components/cmp_text.h"
#include "../entities/camera.h"
#include "../entities/planet.h"
#include "../entities/button.h"
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

void Settings::MoveToMenu()
{
	Engine::ChangeScene((Scene*)&menu);
}

void Settings::Load()
{
	//create camera view
	auto camera = makeEntity<Camera>();
	camera->setPosition(Vector2f(1000.0f, 500.0f));
	camera->setScale(1.0f);

	// Create planet
	float planetRadius = Engine::GetWindowSize().x / 2;
	float planetMass = 1000000.0f;
	auto planet = makeEntity<Planet>(planetRadius, planetMass);

	// Create buttons
	Vector2f buttonSize(400.0f, 75.0f);

	// Header
	_head = makeEntity<Button>(Vector2f(500.0f, 60.0f));
	_head->setText("Settings");
	_head->setPosition(Vector2f(Engine::GetWindowSize().x / 2, Engine::GetWindowSize().y / 2 - 400));
	_head->setActive(false);

	// Back Button
	_menu = makeEntity<Button>(buttonSize);
	_menu->onButtonClicked = FButtonClicked::from_function<Settings, &Settings::MoveToMenu>(this);
	_menu->setPosition(Vector2f(Engine::GetWindowSize().x / 2, Engine::GetWindowSize().y / 2 +300));
	_menu->setText("Back");

	SetLoaded(true);
}

void Settings::UnLoad()
{
	Scene::UnLoad();
}

void Settings::Update(const double& dt)
{
	Scene::Update(dt);
}
