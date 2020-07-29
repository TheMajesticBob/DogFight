#include "system_renderer.h"
#include "system_ui.h"
#include <queue>

using namespace std;
using namespace sf;

typedef pair<int, queue<const Drawable*>> RenderQueuePair;

static map<int, queue<const Drawable*>> sprites;
static RenderWindow* rw;
static RenderTexture rt;

void Renderer::initialise(sf::RenderWindow& r) 
{
	rw = &r; 
	Vector2u windowSize = rw->getSize();

	// Create rendertarget for UI
	rt.create(windowSize.x, windowSize.y);
}

void Renderer::shutdown()
{
	for (auto& spriteQueue : sprites)
	{
		while (!spriteQueue.second.empty())
		{
			spriteQueue.second.pop();
		}
	}

	sprites.clear();

	UI::shutdown();
}

void Renderer::update(const double& dt) { UI::update(dt); }

void Renderer::render() 
{
	if (rw == nullptr) 
	{
		throw("No render window set! ");
	}

	// Get reference to queues to avoid copying data and slowing down the process
	for (auto& spriteQueue : sprites)
	{
		while (!spriteQueue.second.empty())
		{
			rw->draw(*spriteQueue.second.front());
			spriteQueue.second.pop();
		}
	}

	// Draw the UI
	{
		// Render all of UI queue to the rendertarget
		UI::render(rt);
	
		// Get UI rentertarget texture and apply it to a new sprite
		sf::Sprite uiSprite(rt.getTexture());
	
		// Get main camera (renderwindow) view to determine the UI position in world-space and set the sprite position
		View mainView = rw->getView();
		uiSprite.setPosition(mainView.getCenter() - (mainView.getSize() / 2.0f));

		// Calculate UI scale in case the view size is not default
		sf::Vector2f uiScale = sf::Vector2f(mainView.getSize().x / rw->getDefaultView().getSize().x, mainView.getSize().y / rw->getDefaultView().getSize().y);
		uiSprite.setScale(uiScale);

		// Draw UI
		rw->draw(uiSprite);
	}
}

void Renderer::queue(const sf::Drawable* s) { sprites[0].push(s); }

void Renderer::queue(const FDrawable* s)
{
	// Make sure we have enough layers
// 	if(s->layer >= sprites.size())
// 	{
// 		sprites.resize(s->layer + 1);
// 	}

	sprites[s->layer].push(s->drawable.get());
}
