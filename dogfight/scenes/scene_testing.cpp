#include "scene_testing.h"
#include "../game.h"
#include "../entities/player.h"
#include "../entities/enemy.h"
#include "../entities/camera.h"
#include "../entities/planet.h"
#include "../controllers/shipPlayerController.h"
#include "../ai/ai_basicbehaviourtree.h"
#include "scene_menuMain.h"

#include "pool.h"
#include "../entities/projectile.h"

#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

void TestingScene::Load()
{
	Pool<Projectile>::Initialize(0, true, [&] { return makeEntity<Projectile>(); });

	float planetRadius = 1000.0f;
	float planetMass = 5000.0f;

	// Create a camera
	_camera = makeEntity<FollowCamera>();

	// Create the planet
	_planet = makeEntity<Planet>(planetRadius, planetMass);
	_camera->AddFollow(_planet, 0.2f);

	_player = makeEntity<Player>("player");
	_player->GetMovementComponent()->teleport(Vector2f(planetRadius, planetRadius + 100.0f));
	_camera->AddFollow(_player, 10);

	auto controller = makeEntity<ShipPlayerController>("Player1controls",_player.get());	

	auto enemy = makeEntity<Enemy>("basic");
	enemy->GetMovementComponent()->teleport(_player->getPosition() + Vector2f(50.0f,0.0f));
	_camera->AddFollow(enemy, 1);

	enemy = makeEntity<Enemy>("bomber");
	enemy->GetMovementComponent()->teleport(_player->getPosition() + Vector2f(150.0f, 0.0f));
	_camera->AddFollow(enemy, 1);

	Vector2f boxSizes[] = 
	{
		{(float)Engine::getWindowSize().x * 3.0f, 10.0f},
		{(float)Engine::getWindowSize().x * 3.0f, 10.0f},
		{10.0f, (float)Engine::getWindowSize().y},
		{10.0f, (float)Engine::getWindowSize().y}
	};

	Vector2f boxPositions[] = 
	{
	    {Engine::getWindowSize().x * 3.0f / 2.0f, 5.0f},
	    {Engine::getWindowSize().x * 3.0f / 2.0f, Engine::getWindowSize().y - 5.0f},
	    {Engine::getWindowSize().x * 3.0f - 5.0f, Engine::getWindowSize().y / 2.0f},
	    {5.0f, Engine::getWindowSize().y / 2.0f}
	};

	// planet->GetPhysicsComponent()->teleport()

// 	for(int i = 0; i < 4; ++i)
// 	{
// 	    auto box = makeEntity();
// 	    box->setPosition(boxPositions[i]);
// 
// 	    box->addComponent<PhysicsComponent>(false, boxSizes[i]);
// 	    auto shape = box->addComponent<ShapeComponent>();
// 		shape->setShape<RectangleShape>(boxSizes[i]);
// 		shape->getShape().setFillColor(Color::White);
// 		shape->getShape().setOrigin(boxSizes[i] / 2.0f);
// 	}

	setLoaded(true);
}

void TestingScene::Update(const double& dt)
{
	if (Keyboard::isKeyPressed(Keyboard::BackSpace)) {
		Engine::ChangeScene((Scene*)&menu);
	}

	Scene::Update(dt);
}

void TestingScene::UnLoad()
{
	cout << "Eng: Game Scene Unload" << endl;
	if (_camera != nullptr) {
		_camera->setForDelete();
	}	
	
	Scene::UnLoad();
}
