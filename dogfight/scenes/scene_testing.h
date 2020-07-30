#pragma once
#include <engine.h>

class TestingScene : public Scene {
public:
  TestingScene() = default;
  ~TestingScene() override = default;

  void Load() override;

  void Update(const double& dt) override;

  void UnLoad() override;

private:
  std::shared_ptr<class FollowCamera> _camera;
  std::shared_ptr<class Player> _player;
  std::shared_ptr<class Planet> _planet;
};
