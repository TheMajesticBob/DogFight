#include "system_renderer.h"
#include "system_ui.h"
#include <queue>
#include <vector>

using namespace std;
using namespace sf;

typedef pair<int, queue<const Drawable*>> RenderQueuePair;

static std::vector<Renderer::FPostProcessEffect> _postProcessEffects;
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
	renderTarget_Main.setView(rw->getView());
	
	// Draw stuff to the main render target
	// Get reference to queues to avoid copying data and slowing down the process
	for (auto& spriteQueue : sprites)
	{
		while (!spriteQueue.second.empty())
		{
			renderTarget_Main.draw(*spriteQueue.second.front());
			//rw->draw(*spriteQueue.second.front());
			spriteQueue.second.pop();
		}
	}

	// Draw the main stuff and apply post processing layers
	{
		const View& mainView = rw->getView();
		sf::Vector2f viewScale = sf::Vector2f(mainView.getSize().x / rw->getSize().x, -mainView.getSize().y / rw->getSize().y);
		sf::Vector2f viewPosition = sf::Vector2f((mainView.getCenter() - mainView.getSize() / 2.0f).x, (mainView.getCenter() + mainView.getSize() / 2.0f).y);
		// We have to flip the Y axis when drawing a rendertarget for some reason... This makes sure the visible stuff represents the actual game world

		// NOTE - the post processing effects will draw in the order they've been added!
		for (auto effect : _postProcessEffects)
		{ 
			if (effect.set_for_delete)
			{
				continue;
			}

			// Get a copy of main rendertarget texture and draw it with specified effect. 
			sf::Texture mainTexture = renderTarget_Main.getTexture();
			sf::Sprite mainSprite(mainTexture);
			// Set the position and scale calculated before. This will draw the rendertarget in the exact same position as the viewport.
			mainSprite.setPosition(viewPosition);
			mainSprite.setScale(viewScale);
			// Make sure to clear the main RT before drawing new stuff on it.
			renderTarget_Main.clear();
			renderTarget_Main.draw(mainSprite, effect.effect);
		}

		// Get the main rentertarget texture and draw it on the screen.
		sf::Texture mainTexture = renderTarget_Main.getTexture();
		sf::Sprite mainSprite(mainTexture);
		mainSprite.setPosition(viewPosition); 
		mainSprite.setScale(viewScale);
		rw->draw(mainSprite);
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

void Renderer::add_postprocess_effect(Renderer::FPostProcessEffect& effect)
{
	_postProcessEffects.push_back(effect);
}

void Renderer::remove_postprocess_effect(Renderer::FPostProcessEffect& effect)
{
	auto it = std::find(_postProcessEffects.begin(), _postProcessEffects.end(), effect);
	if (it != _postProcessEffects.end())
	{
		it->set_for_delete = true;
	}

	_postProcessEffects.erase(std::remove(_postProcessEffects.begin(), _postProcessEffects.end(), effect));
}

void Renderer::queue(const FDrawable* s)
{
	sprites[s->layer].push(s->drawable.get());
}
