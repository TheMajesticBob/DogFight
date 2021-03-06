#pragma once
#include <engine.h>
#include "../engine/game_resources.h"

class TestingScene : public Scene {
public:
  TestingScene() = default;
  ~TestingScene() override = default;

  void Load() override;

  void Update(const double& dt) override;

  void UnLoad() override;

private:
	void StartNextWave();
	void LevelCompleted();

	void DetachShipFromCamera(std::shared_ptr<class Entity> ship);
	void OnShipDestroyed(std::shared_ptr<class Entity> ship);

	std::shared_ptr<class ExplosionManager> _explosionManager;

	std::shared_ptr<defs::Level> _currentLevel;
	std::vector<defs::Wave> _waveData;
	defs::Wave currentWave;

	uint8 _waveId;
	float _timeToNextWave;
	bool _waveRunning = false;

  std::shared_ptr<class FollowCamera> _camera;
  std::shared_ptr<class Player> _player;
  std::shared_ptr<class Player> _player2;
  std::shared_ptr<class Planet> _planet;

  std::shared_ptr<class TextComponent> _waveText;
};
