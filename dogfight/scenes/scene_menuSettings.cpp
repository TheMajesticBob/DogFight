#include "scene_menuSettings.h"
#include "../game.h"
#include "system_resources.h"
#include "../components/cmp_text.h"
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

void Settings::Load()
{

	auto text_mainTitle = makeEntity();
	auto t = text_mainTitle->addComponent<TextComponent>("      Settings");
}

void Settings::Update(const double& dt)
{
	if (Keyboard::isKeyPressed(Keyboard::BackSpace)) {
		Engine::ChangeScene((Scene*)&menu);
	}
}
