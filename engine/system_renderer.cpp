#include "system_renderer.h"
#include <queue>

using namespace std;
using namespace sf;

static queue<const Drawable*> sprites;
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
	while (!sprites.empty())
	{
		sprites.pop();
	}

	UI::shutdown();
}

void Renderer::update(const double& dt) { UI::update(dt); }

void Renderer::render() 
{
	if (rw == nullptr) 
	{
		throw("No render window set! ");
	}

	while (!sprites.empty()) 
	{
		rw->draw(*sprites.front());
		sprites.pop();
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
	
		// Draw UI
		rw->draw(uiSprite);
	}
}

void Renderer::queue(const sf::Drawable* s) { sprites.push(s); }