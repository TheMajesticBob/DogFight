#include "scene_mainMenu.h"
#include "../game.h"
#include "../entities/planet.h"
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;



void MainMenu::Load()
{
	float planetRadius = Engine::getWindowSize().x/2;
	float planetMass = 1000000.0f;

	// Create planet
	auto planet = makeEntity<Planet>(planetRadius, planetMass);	

	if (!font_mainTitle.loadFromFile("res/fonts/Misdemeanor-Oyx8.ttf"))
	{
		std::cout << "Couldn't load font!" << std::endl;
	}

	text_mainTitle.setFont(font_mainTitle);
	text_mainTitle.setCharacterSize(24);

	setLoaded(true);
}

void MainMenu::Update(const double& dt)
{
	if (Keyboard::isKeyPressed(Keyboard::Num1)) {
		Engine::ChangeScene((Scene*)&testing);
	}
	Scene::Update(dt);
}
