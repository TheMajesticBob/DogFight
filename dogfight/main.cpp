#include <engine.h>
#include "game.h"
#include "scenes/scene_testing.h"
#include "scenes/scene_menuMain.h"
#include "scenes/scene_menuSettings.h"

using namespace std;

MainMenu menu;
TestingScene testing;
Settings settings;

int main(int argc, char** argv)
{
	Engine::Start(1920, 1080, "DogFight", &menu);

	return 0;
}