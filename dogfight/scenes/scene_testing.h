#pragma once
#include <engine.h>

class TestingScene : public Scene {
public:
  TestingScene() = default;
  ~TestingScene() override = default;

  void Load() override;

  void Update(const double& dt) override;

  void UnLoad() override;


  std::shared_ptr<class BasicBehaviourTree> bt;
  std::shared_ptr<class Camera> _camera;
};
