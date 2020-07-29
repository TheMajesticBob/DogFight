#include "system_ui.h"
#include "system_renderer.h"
#include <queue>

static std::map<int, std::queue<const sf::Drawable*>> ui_sprites;

void UI::shutdown()
{
	for (auto& spriteQueue : ui_sprites)
	{
		while (!spriteQueue.second.empty())
		{
			spriteQueue.second.pop();
		}
	}
}

void UI::update(const double&)
{

}

void UI::queue(const sf::Drawable* s)
{
	ui_sprites[0].push(s);
}

void UI::queue(const FDrawable* s)
{
	ui_sprites[s->layer].push(s->drawable.get());
}

void UI::render(sf::RenderTexture& rt)
{
	// Clear the UI with a transparent color
	rt.clear(sf::Color(0,0,0,0));

	for (auto& spriteQueue : ui_sprites)
	{
		while (!spriteQueue.second.empty())
		{
			rt.draw(*spriteQueue.second.front());
			spriteQueue.second.pop();
		}
	}

// 	// Draw all drawables in the queue
// 	while (!m_sprites.empty())
// 	{
// 		rt.draw(*m_sprites.front());
// 		m_sprites.pop();
// 	}

	// Display is required in order for the rendertarget to render properly later
	rt.display();
}