#include <engine.h>
#include "game.h"
#include "scenes/scene_menu.h"

using namespace std;

MenuScene menu;

int main(int argc, char** argv)
{
	Engine::Start(1280, 720, "DogFight", &menu);

	return 0;
}