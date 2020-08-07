#pragma once
#include <ecm.h>
#include "../planet_protector/definitions.h"

class SpawnPoint : public Entity
{
public:
	SpawnPoint() = delete;
	SpawnPoint(class Scene* const s);

	void update(double dt) override;

	void SetActive(bool active) { _isActive = active; }
	void Initialize(std::shared_ptr<defs::Spawner> definition);

protected:
	void SpawnGroup();

	std::shared_ptr<defs::Spawner> _spawnDefinition;

	std::vector<defs::EnemyGroup> _shipsRemaining;
	float delayBetweenSpawns;
	float initialDelay;
	float _currentDelay = 0;
	int maxShipsPerSpawn;

	bool _isActive;
};