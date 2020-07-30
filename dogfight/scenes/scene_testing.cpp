#include "scene_testing.h"
#include "../game.h"
#include "../entities/player.h"
#include "../entities/enemy.h"
#include "../entities/camera.h"
#include "../entities/planet.h"
#include "../controllers/shipPlayerController.h"
#include "../ai/ai_basicbehaviourtree.h"
#include "scene_menuMain.h"
#include "../engine/game_resources.h"

#include "pool.h"
#include "../entities/projectile.h"

#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

void TestingScene::Load()
{
	Pool<Projectile>::Initialize(0, true, [&] { return makeEntity<Projectile>(); });

	_currentLevel = Resources::get<defs::Level>("1");
	_waveData = _currentLevel->waves;
	_waveId = 0;
	_timeToNextWave = 0.0f;
	
	float planetRadius = 1000.0f;
	float planetMass = 5000.0f;

	// Create a camera
	_camera = makeEntity<FollowCamera>();

	// Create the planet
	_planet = makeEntity<Planet>(_currentLevel->planetRadius, _currentLevel->planetMass);
	_camera->AddFollow(_planet, 0.2f);

	_player = makeEntity<Player>("player");
	_player->GetMovementComponent()->teleport(Vector2f(planetRadius, planetRadius + 100.0f));
	_player->SetGodMode(true);
	_camera->AddFollow(_player, 10);

	auto controller = makeEntity<ShipPlayerController>("Player1controls",_player.get());	

// 	auto enemy = makeEntity<Enemy>("basic");
// 	enemy->GetMovementComponent()->teleport(_player->getPosition() + Vector2f(50.0f,0.0f));
// 	enemy->SetTeam(Ship::Team::T_ENEMY);
// 	_camera->AddFollow(enemy, 1);
// 
// 	enemy = makeEntity<Enemy>("bomber");
// 	enemy->GetMovementComponent()->teleport(_player->getPosition() + Vector2f(150.0f, 0.0f));
// 	enemy->SetTeam(Ship::Team::T_ENEMY);
// 	_camera->AddFollow(enemy, 1);

	StartNextWave();

	setLoaded(true);
}

void TestingScene::Update(const double& dt)
{
	if (Keyboard::isKeyPressed(Keyboard::BackSpace)) {
		Engine::ChangeScene(&menu, true);
	}

	if (_currentLevel != nullptr && _waveRunning)
	{
		_timeToNextWave -= dt;
		if(_timeToNextWave <= 0.0f)
		{
			StartNextWave();
		}

		for (auto& s : currentWave.spawners)
		{
			s.currentDelay -= dt;
			if (s.currentDelay <= 0.0f)
			{
				{
					// Spawn enemies
					for (int i = 0; i < s.maxShipsPerSpawn; ++i)
					{
						std::vector<std::string> ships;
						for (auto& eg : s.enemyGroups)
						{
							for (int j = 0; j < eg.count; ++j)
							{
								ships.push_back(eg.shipName);
							}
						}

						if (ships.size() > 0)
						{
							int randomShip = rand() % ships.size();
							std::string shipName = ships[randomShip];
							for (auto& eg : s.enemyGroups)
							{
								if (eg.shipName == shipName)
								{
									--eg.count;
									break;
								}
							}

							sf::Vector2f randomOffset((rand() % 2000 - 1000) / 10.0f, (rand() % 2000 - 1000) / 10.0f);

							auto enemy = makeEntity<Enemy>(shipName);
							enemy->GetMovementComponent()->teleport(s.position + randomOffset);
							enemy->SetTeam(Ship::Team::T_ENEMY);
							//_camera->AddFollow(enemy, 0.05f);
						}
					}
				}

				s.currentDelay += s.delayBetweenSpawns;
			}
		}
	}

	Scene::Update(dt);
}

void TestingScene::UnLoad()
{
	cout << "Eng: Game Scene Unload" << endl;
	if (_camera != nullptr) {
		_camera->setForDelete();
	}

	ents.list.clear();

	Scene::UnLoad();
}

void TestingScene::StartNextWave()
{
	if (_currentLevel)
	{
		if (_currentLevel->waves.size() > _waveId++)
		{
			// Get current wave data
			currentWave = _currentLevel->waves[_waveId - 1];

			// Setup spawners
			for (auto& s : currentWave.spawners)
			{
				float angle = std::rand() % 360;
				s.position = sf::Vector2f(cos(deg2rad(angle)), sin(deg2rad(angle))) * _currentLevel->planetRadius*1.5f;
				s.currentDelay = s.initialDelay;
			}

			// Setup time for next wave
			if (_currentLevel->waves.size() > _waveId)
			{
				_timeToNextWave = _currentLevel->waves[_waveId].delay;
			}

			// Wave is currently running
			_waveRunning = true;
		}
		else
		{
			LevelCompleted();
		}
	}
}

void TestingScene::LevelCompleted()
{
	_waveId = 0;
}
