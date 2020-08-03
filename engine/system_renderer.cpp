#include "system_renderer.h"
#include "system_ui.h"
#include <queue>
#include <vector>

using namespace std;
using namespace sf;

typedef pair<int, queue<const Drawable*>> RenderQueuePair;

static std::vector<sf::Shader*> _postProcessEffects;
static map<int, queue<const Drawable*>> sprites;
static RenderWindow* rw;
static RenderTexture renderTarget_UI;
static RenderTexture renderTarget_Main;

void Renderer::initialise(sf::RenderWindow& r) 
{
	rw = &r; 
	Vector2u windowSize = rw->getSize();

	// Create rendertargets
	renderTarget_Main.create(windowSize.x, windowSize.y);
	renderTarget_UI.create(windowSize.x, windowSize.y);
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

	// Clear main with black
	renderTarget_Main.clear(sf::Color(0, 0, 0, 255));

	// Draw stuff to the main render target
	// Get reference to queues to avoid copying data and slowing down the process
	for (auto& spriteQueue : sprites)
	{
		while (!spriteQueue.second.empty())
		{
			//renderTarget_Main.draw(*spriteQueue.second.front());
			rw->draw(*spriteQueue.second.front());
			spriteQueue.second.pop();
		}
	}

	// Draw the main stuff and apply post processing layers
	{
		for (auto effect : _postProcessEffects)
		{
			// Get UI rentertarget texture and apply it to a new sprite
			sf::Texture mainTexture = renderTarget_Main.getTexture();
			sf::Sprite mainSprite(mainTexture);

			View mainView = rw->getView();
			//mainSprite.setPosition(mainView.getCenter() - (mainView.getSize() / 2.0f));

			renderTarget_Main.clear();
			renderTarget_Main.draw(mainSprite, effect);
		}

		// Get UI rentertarget texture and apply it to a new sprite
		sf::Sprite mainSprite(renderTarget_Main.getTexture());

		// Draw post processed main screen
		//rw->draw(mainSprite);
	}

	// Draw the UI
	{
		// Render all of UI queue to the rendertarget
		UI::render(renderTarget_UI);
	
		// Get UI rentertarget texture and apply it to a new sprite
		sf::Sprite uiSprite(renderTarget_UI.getTexture());
	
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

void Renderer::queue(const sf::Drawable* s, int layer) { sprites[layer].push(s); }

void Renderer::add_postprocess_effect(sf::Shader* effect)
{
	_postProcessEffects.push_back(effect);
}

void Renderer::remove_postprocess_effect(sf::Shader* effect)
{
	std::remove(_postProcessEffects.begin(), _postProcessEffects.end(), effect);
}

void Renderer::queue(const FDrawable* s)
{
	// Make sure we have enough layers
// 	if(s->layer >= sprites.size())
// 	{
// 		sprites.resize(s->layer + 1);
// 	}

	sprites[s->layer].push(s->drawable.get());
}
