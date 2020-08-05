#include <engine.h>
#include "game.h"
#include "scenes/scene_editor.h"
#include "scenes/scene_testing.h"
#include "scenes/scene_menuMain.h"
#include "scenes/scene_menuSettings.h"
#include <windows.h>

using namespace std;

MainMenu menu;
TestingScene testing;
Settings settings;
EditorScene editorScene;

std::shared_ptr<class AmbientManager> _ambientManager;

int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow)
{
	Engine::Start(GAME_WIDTH, GAME_HEIGHT, "DogFight", &menu);

	return 0;
}

int main(int argc, char** argv)
{
	Engine::Start(GAME_WIDTH, GAME_HEIGHT, "DogFight", &menu);

	return 0;
}