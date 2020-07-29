#include "scene_menuMain.h"
#include "../game.h"
#include "../entities/planet.h"
#include "system_resources.h"
#include "../components/cmp_text.h"
#include <SFML/Graphics.hpp>
#include "../entities/camera.h"
#include "../entities/button.h"

using namespace sf;
using namespace std;

void MainMenu::Load()
{
	//create camera view
	auto camera = makeEntity<Camera>();
	camera->setPosition(Vector2f(1000.0f, 500.0f));
	camera->setScale(1.0f);
	
	// Create planet
	float planetRadius = Engine::getWindowSize().x / 2;
	float planetMass = 1000000.0f;
	auto planet = makeEntity<Planet>(planetRadius, planetMass);	

	// Create Button
	Vector2f buttonSize(400.0f, 75.0f);

	auto _play = makeEntity<Button>(buttonSize);
	_play->setPosition(Vector2f(Engine::getWindowSize().x / 2, Engine::getWindowSize().y/2));
	
	// Create menu text
	auto text_mainTitle = makeEntity();	
	auto t = text_mainTitle->addComponent<TextComponent>("\n\n\n\n\n\n\n\n\n\n\n\n                                                1 - Play\n                                               2 - Settings");

	auto text_menu1 = makeEntity();
	auto a = text_menu1->addComponent<TextComponent>("\n\n\n\n\n\n\n\n\n\n                                                  ~PLANET PROTECTOR~");
	text_mainTitle->setPosition(Vector2f(Engine::getWindowSize().x / 2, Engine::getWindowSize().y));

	setLoaded(true);
}

void MainMenu::UnLoad()
{
	Scene::UnLoad();
}

void MainMenu::Update(const double& dt)
{	
	if (Keyboard::isKeyPressed(Keyboard::Num1)) {
		Engine::ChangeScene((Scene*)&testing);
	}
	if (Keyboard::isKeyPressed(Keyboard::Num2)) {
		Engine::ChangeScene((Scene*)&settings);
	}

	Scene::Update(dt);
}
