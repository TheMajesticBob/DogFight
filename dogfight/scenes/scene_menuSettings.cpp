#include "scene_menuSettings.h"
#include "../game.h"
#include "system_resources.h"
#include "../components/cmp_text.h"
#include "../entities/camera.h"
#include "../entities/planet.h"
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
	

	auto text_mainTitle = makeEntity();
	auto t = text_mainTitle->addComponent<TextComponent>("Settings");
}

void Settings::Update(const double& dt)
{
	if (Keyboard::isKeyPressed(Keyboard::BackSpace)) {
		Engine::ChangeScene((Scene*)&menu);
	}
}
