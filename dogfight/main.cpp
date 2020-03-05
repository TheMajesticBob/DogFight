#include <engine.h>
#include "game.h"
#include "scenes/scene_testing.h"

using namespace std;

TestingScene testing;

int main(int argc, char** argv)
{
	Engine::Start(1920, 1080, "DogFight", &testing);

	return 0;
}