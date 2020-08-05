#include "scene_testing.h"
#include "../game.h"
#include "../entities/player.h"
#include "../entities/enemy.h"
#include "../entities/camera.h"
#include "../entities/planet.h"
#include "../controllers/shipPlayerController.h"
#include "../ai/ai_basicbehaviourtree.h"
#include "../engine/game_resources.h"
#include "../entities/explosion.h"

#include "pool.h"
#include "../entities/projectile.h"

#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

void TestingScene::Load()
{
	Pool<Projectile>::Initialize(0, true, [&] { return MakeEntity<Projectile>(); });

	_currentLevel = Resources::get<defs::Level>("1");
	_waveData = _currentLevel->waves;
	_waveId = 0;
	_timeToNextWave = 0.0f;
	
	float planetRadius = 1000.0f;
	float planetMass = 5000.0f;

	// Create a camera
	_camera = MakeEntity<FollowCamera>();

	// Create the planet
	_planet = MakeEntity<Planet>(_currentLevel->planetRadius, _currentLevel->planetMass);
	_camera->AddFollow(_planet, 0.2f);

	_player = MakeEntity<Player>("player");
	_player->GetMovementComponent()->teleport(Vector2f(planetRadius, planetRadius + 100.0f));
	_player->onShipDestroyed = FShipDestroyed::from_function<TestingScene, &TestingScene::DetachShipFromCamera>(this);
	_player->onShipDestroyed += FShipDestroyed::from_function<TestingScene, &TestingScene::OnShipDestroyed>(this);
	_player->SetGodMode(true);
	_camera->AddFollow(_player, 10);

	_player2 = MakeEntity<Player>("player");
	_player2->GetMovementComponent()->teleport(Vector2f(planetRadius, planetRadius + 130.0f));
	_player2->onShipDestroyed = FShipDestroyed::from_function<TestingScene, &TestingScene::DetachShipFromCamera>(this);
	_player2->onShipDestroyed += FShipDestroyed::from_function<TestingScene, &TestingScene::OnShipDestroyed>(this);
	//_player2->SetGodMode(true);
	_camera->AddFollow(_player2, 10);

	auto text = MakeEntity();
	text->setPosition({ 50.0f, 50.0f });
	_waveText = text->addComponent<TextComponent>();
	_waveText->setDrawOnUI(true);
	_waveText->SetText("Wave 0");

	auto controller = MakeEntity<ShipPlayerController>("Player1controls", _player.get());
	auto controller2 = MakeEntity<ShipPlayerController>("Player2controls", _player2.get());

	_explosionManager = MakeEntity<ExplosionManager>();

	StartNextWave();

	SetLoaded(true);
}

void TestingScene::Update(const double& dt)
{
	if (Keyboard::isKeyPressed(Keyboard::BackSpace)) {
		Engine::ChangeScene(&menu);
	}

	if (_currentLevel != nullptr && _waveRunning)
	{
		_timeToNextWave -= dt;
		if(_timeToNextWave <= 0.0f)
		{
			StartNextWave();
		}

		if (_waveText)
		{
			std::string newText = "Wave " + std::to_string(_waveId) + "\nNext wave in: " + std::to_string((int)_timeToNextWave);
			_waveText->SetText(newText);
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

							auto enemy = MakeEntity<Enemy>(shipName);
							enemy->GetMovementComponent()->teleport(s.position + randomOffset);
							enemy->onShipDestroyed = FShipDestroyed::from_function<TestingScene, &TestingScene::OnShipDestroyed>(this);
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

	_camera.reset();
	_waveText.reset();
	_planet.reset();
	_player.reset();
	_player2.reset();
	_waveText.reset();
	_explosionManager.reset();

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

void TestingScene::DetachShipFromCamera(std::shared_ptr<class Entity> ship)
{
	if (ship != nullptr)
	{
		_camera->RemoveFollow(ship);
	}
}

void TestingScene::OnShipDestroyed(std::shared_ptr<class Entity> ship)
{
	if (ship != nullptr)
	{
		_explosionManager->Fire(ship->getPosition(), sf::Vector2f(300.0f, 300.0f), 4.0f);
	}
}
