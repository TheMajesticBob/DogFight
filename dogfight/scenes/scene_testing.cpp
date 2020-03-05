#include "scene_testing.h"
#include "../game.h"
#include "../entities/player.h"
#include "../entities/camera.h"

#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

void TestingScene::Load()
{
  auto player = makeEntity<Player>();
  player->GetMovementComponent()->teleport(Vector2f(Engine::GetWindow().getSize().x / 2, Engine::GetWindow().getSize().y / 2));

  auto camera = makeEntity<Camera>();
  camera->AddFollow(player);

  Vector2f boxSizes[] = {
    {(float)Engine::getWindowSize().x * 3.0f, 10.0f},
    {(float)Engine::getWindowSize().x * 3.0f, 10.0f},
    {10.0f, (float)Engine::getWindowSize().y},
    {10.0f, (float)Engine::getWindowSize().y}
  };

  Vector2f boxPositions[] = {
    {Engine::getWindowSize().x * 3.0f / 2.0f, 5.0f},
    {Engine::getWindowSize().x * 3.0f / 2.0f, Engine::getWindowSize().y - 5.0f},
    {Engine::getWindowSize().x * 3.0f - 5.0f, Engine::getWindowSize().y / 2.0f},
    {5.0f, Engine::getWindowSize().y / 2.0f}
  };

  for(int i = 0; i < 4; ++i)
  {
    auto box = makeEntity();
    box->setPosition(boxPositions[i]);

    box->addComponent<PhysicsComponent>(false, boxSizes[i]);
    auto shape = box->addComponent<ShapeComponent>();
    shape->setShape<RectangleShape>(boxSizes[i]);
    shape->getShape().setFillColor(Color::White);
    shape->getShape().setOrigin(boxSizes[i] / 2.0f);
  }

  setLoaded(true);
}

void TestingScene::Update(const double& dt)
{
  Scene::Update(dt);
}