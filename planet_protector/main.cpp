#include <engine.h>
#include "game.h"
#include "scenes/scene_testing.h"
#include "scenes/scene_menuMain.h"
#include "scenes/scene_menuSettings.h"

using namespace std;

MainMenu menu;
TestingScene testing;
Settings settings;

std::shared_ptr<class AmbientManager> _ambientManager;

int main(int argc, char** argv)
{
	Engine::Start(GAME_WIDTH, GAME_HEIGHT, "DogFight", &menu);

	return 0;
}