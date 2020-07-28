#include <engine.h>
#include "game.h"
#include "scenes/scene_testing.h"
#include "scenes/scene_mainMenu.h"

using namespace std;

MainMenu menu;
TestingScene testing;

int main(int argc, char** argv)
{
	Engine::Start(1920, 1080, "DogFight", &menu);

	return 0;
}