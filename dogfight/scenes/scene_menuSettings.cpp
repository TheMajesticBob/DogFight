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

void Settings::Load()
{
	//create camera view
	auto camera = makeEntity<Camera>();
	camera->setPosition(Vector2f(1000.0f, 500.0f));
	camera->setScale(1.0f);

	// Create planet
	float planetRadius = Engine::getWindowSize().x / 2;
	float planetMass = 1000000.0f;
	auto planet = makeEntity<Planet>(planetRadius, planetMass);

	//create button
	Vector2f buttonSize(400.0f, 75.0f);

	_menu = makeEntity<Button>(buttonSize);
	_menu->setPosition(Vector2f(Engine::getWindowSize().x / 2, Engine::getWindowSize().y / 2));
	_menu->setText("Back");
	

	auto text_mainTitle = makeEntity();
	auto t = text_mainTitle->addComponent<TextComponent>("Settings");
}

void Settings::UnLoad()
{
	Scene::UnLoad();
}

void Settings::Update(const double& dt)
{
	if (_menu->MouseClick()) {
		Engine::ChangeScene((Scene*)&menu);
	}
}
