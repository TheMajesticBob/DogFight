#include "engine.h"
#include "maths.h"
#include "system_physics.h"
#include "system_renderer.h"
#include "system_resources.h"
#include "input_handler.h"
#include "system_ui.h"
#include <SFML/Graphics.hpp>
#include <future>
#include <iostream>
#include <stdexcept>

using namespace sf;
using namespace std;
Scene* Engine::_activeScene = nullptr;
std::string Engine::_gameName;
double Engine::_framerateCap = 1 / 200.0;

static bool loading = false;
static float loadingspinner = 0.f;
static float loadingTime;
static RenderWindow* _window;

void Loading_update(float dt, const Scene* const scn) {
  //  cout << "Eng: Loading Screen\n";
  if (scn->isLoaded()) {
    cout << "Eng: Exiting Loading Screen\n";
    loading = false;
  } else {
    loadingspinner += 220.0f * dt;
    loadingTime += dt;
  }
}
void Loading_render() {
  // cout << "Eng: Loading Screen Render\n";
  static CircleShape octagon(80, 8);
  octagon.setOrigin(80, 80);
  octagon.setRotation(loadingspinner);
  octagon.setPosition(Vcast<float>(Engine::getWindowSize()) * .5f);
  octagon.setFillColor(Color(255,255,255,min(255.f,40.f*loadingTime)));
  static Text t("Loading", *Resources::get<sf::Font>("RobotoMono-Regular.ttf"));
  t.setFillColor(Color(255,255,255,min(255.f,40.f*loadingTime)));
  t.setPosition(Vcast<float>(Engine::getWindowSize()) * Vector2f(0.4f,0.3f));
  Renderer::queue(&t);
  Renderer::queue(&octagon);
}

float frametimes[256] = {};
uint8_t ftc = 0;

void Engine::Update(double dt) {
//   static sf::Clock clock;
//   float dt = clock.restart().asSeconds();
  {
    frametimes[++ftc] = dt;
    static string avg = _gameName + " FPS:";
    if (ftc % 60 == 0) {
      double davg = 0;
      for (const auto t : frametimes) {
        davg += t;
      }
      davg = 1.0 / (davg / 255.0);
      _window->setTitle(avg + toStrDecPt(2, davg));
    }
  }

  if (loading) {
    Loading_update(dt, _activeScene);
  } else if (_activeScene != nullptr) 
  {
	  InputHandler::Update(_framerateCap);
	  Physics::update(dt);
	  _activeScene->Update(dt);
  }
}

void Engine::Render(RenderWindow& window) {
  if (loading) {
    Loading_render();
  } else if (_activeScene != nullptr) {
    _activeScene->Render();
  }

  Renderer::render();

  if (Physics::GetWorld())
  {
	  Physics::GetWorld()->DrawDebugData();
  }
}

void Engine::Start(unsigned int width, unsigned int height,
                   const std::string& gameName, Scene* scn) {
	sf::VideoMode videoMode(width, height);
	
	RenderWindow window(videoMode, gameName, sf::Style::Titlebar | sf::Style::Close);
	_gameName = gameName;
	_window = &window;
	// _window->setFramerateLimit(120);
	Renderer::initialise(window);
	Physics::initialise();
	ChangeScene(scn);

	static sf::Clock clock;
	float firstFrame = clock.restart().asSeconds();
	_framerateCap = 1 / 200.0;

	double currentTime = clock.getElapsedTime().asSeconds();
	double acc = 0;

	while (window.isOpen()) 
	{
		if (acc < 0)
		{
			acc = 0;
		}

		double newTime = clock.getElapsedTime().asSeconds();
		double frameTime = newTime - currentTime;
		currentTime = newTime;

	    Event event;
	    while (window.pollEvent(event)) 
		{
			if (event.type == Event::Closed) 
			{
				window.close();
			}

			if (event.type == Event::KeyReleased && event.key.code == Keyboard::Tilde)
			{
				Physics::setDebugDraw(!Physics::getDebugDraw());
			}

			if (event.type == Event::KeyPressed)
			{
				InputHandler::KeyboardHandler(event.key.code, 0, Event::KeyPressed, 0);
			}
			if (event.type == Event::KeyReleased)
			{
				InputHandler::KeyboardHandler(event.key.code, 0, Event::KeyReleased, 0);
			}
		}
// 		if (Keyboard::isKeyPressed(Keyboard::Escape)) 
// 		{
// 			window.close();
// 		}

		window.clear();
		
		acc += frameTime;
		while (acc >= _framerateCap)
		{
			if (_activeScene->isLoaded())
			{
			}

			Update(_framerateCap);

			acc -= _framerateCap;
		}

		Render(window);
		window.display();
	}
	if (_activeScene != nullptr) 
	{
		_activeScene->UnLoad();
		_activeScene = nullptr;
	}
	window.close();
	Physics::shutdown();
    Renderer::shutdown();
	UI::shutdown();
}

std::shared_ptr<Entity> Scene::makeEntity() {
  auto e = make_shared<Entity>(this);
  ents.list.push_back(e);
  return std::move(e);
}

void Engine::setVsync(bool b) { _window->setVerticalSyncEnabled(b); }

void Engine::ChangeScene(Scene* s, bool forceSync) {
  cout << "Eng: changing scene: " << s << endl;
  auto old = _activeScene;
  _activeScene = s;


  std::queue<std::shared_ptr<Entity>> persistentEntities;

  if (old != nullptr) {

	  for (auto it = old->ents.list.begin(); it != old->ents.list.end(); ++it)
	  {
		  if ((*it)->is_persistent())
		  {
			  persistentEntities.push(*it);
		  }
	  }
    old->UnLoad(); // todo: Unload Async
  }

  _activeScene->addPersistentEntities(persistentEntities);

  if (forceSync)
  {
	  _activeScene->Load();
	  return;
  }

  if (!s->isLoaded()) {
    cout << "Eng: Entering Loading Screen\n";
    loadingTime =0;
    _activeScene->LoadAsync();
    loading = true;
  }  
}

void Scene::Update(const double& dt) { ents.update(dt); }

void Scene::Render() { ents.render(); }

bool Scene::isLoaded() const {
  {
    std::lock_guard<std::mutex> lck(_loaded_mtx);
    // Are we already loading asynchronously?
    if (_loaded_future.valid() // yes
        &&                     // Has it finished?
        _loaded_future.wait_for(chrono::seconds(0)) ==
            future_status::ready) {
      // Yes
      _loaded_future.get();
      _loaded = true;
    }
    return _loaded;
  }
}
void Scene::setLoaded(bool b) {
  {
    std::lock_guard<std::mutex> lck(_loaded_mtx);
    _loaded = b;
  }
}

void Scene::UnLoad() 
{
	setLoaded(false);
	ents.clear();
}

void Scene::LoadAsync() { _loaded_future = std::async(&Scene::Load, this); }

sf::Vector2u Engine::getWindowSize() { return _window->getSize(); }

sf::RenderWindow& Engine::GetWindow() { return *_window; }

namespace timing {
// Return time since Epoc
long long now() {
  return std::chrono::high_resolution_clock::now()
      .time_since_epoch()
      .count();
}
// Return time since last() was last called.
long long last() {
  auto n = now();
  static auto then = now();
  auto dt = n - then;
  then = n;
  return dt;
}
} // namespace timing

Scene::~Scene() { UnLoad(); }
