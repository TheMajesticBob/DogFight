#include "scene_mainMenu.h"
#include "../game.h"
#include "../entities/planet.h"
#include "system_resources.h"
#include "../components/cmp_text.h"
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;



void MainMenu::Load()
{
	float planetRadius = Engine::getWindowSize().x/2;
	float planetMass = 1000000.0f;

	// Create planet
	auto planet = makeEntity<Planet>(planetRadius, planetMass);	
	
	// Create menu text
	auto text_mainTitle = makeEntity();	
	auto t = text_mainTitle->addComponent<TextComponent>("\n\n\n\n\n\n\n\n\n\n\n\n                                                1 - Play\n                                               2 - Settings");

	auto text_menu1 = makeEntity();
	auto a = text_menu1->addComponent<TextComponent>("\n\n\n\n\n\n\n\n\n\n                                                  SPACE WARZ");
	//text_mainTitle->setPosition(Vector2f(Engine::getWindowSize().x / 2, Engine::getWindowSize().y));

	setLoaded(true);
}

void MainMenu::Update(const double& dt)
{	
	if (Keyboard::isKeyPressed(Keyboard::Num1)) {
		Engine::ChangeScene((Scene*)&testing);
	}
	Scene::Update(dt);
}
