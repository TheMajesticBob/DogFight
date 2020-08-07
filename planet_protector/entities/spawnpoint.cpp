#include "spawnpoint.h"
#include "enemy.h"
#include <engine.h>

SpawnPoint::SpawnPoint(class Scene* const s)
	: Entity(s)
{
	_isActive = false;
}

void SpawnPoint::update(double dt)
{
	if (_spawnDefinition && _isActive)
	{
		_currentDelay -= dt;
		if (_currentDelay <= 0.0f)
		{
			SpawnGroup();
			_currentDelay += _spawnDefinition->delayBetweenSpawns;
		}
	}
}

void SpawnPoint::Initialize(std::shared_ptr<defs::Spawner> definition)
{
	_spawnDefinition = definition;
	_shipsRemaining = _spawnDefinition->enemyGroups;
	_currentDelay = 0;
}

void SpawnPoint::SpawnGroup()
{
	// Spawn enemies
	for (int i = 0; i < _spawnDefinition->maxShipsPerSpawn; ++i)
	{
		std::vector<std::string> ships;
		for (auto& eg : _shipsRemaining)
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
			for (auto& eg : _shipsRemaining)
			{
				if (eg.shipName == shipName)
				{
					--eg.count;
					break;
				}
			}

			double angle = sf::deg2rad(sf::rand_range(0.0f, 360.0f));
			double distance = sf::rand_range(10.0f, 50.0f);

			sf::Vector2f randomOffset(cos(angle)*distance,sin(angle)*distance);

			auto enemy = GetScene()->MakeEntity<Enemy>(shipName);
			enemy->GetMovementComponent()->teleport(getPosition() + randomOffset);
			//enemy->onShipDestroyed = FShipDestroyed::from_function<TestingScene, &TestingScene::OnShipDestroyed>(this);
			enemy->SetTeam(Ship::Team::T_ENEMY);
		}
	}
}
