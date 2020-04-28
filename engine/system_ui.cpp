#include "system_ui.h"
#include <queue>

static std::queue<const sf::Drawable*> m_sprites;

void UI::shutdown()
{
	while (!m_sprites.empty())
	{
		m_sprites.pop();
	}
}

void UI::update(const double&)
{

}

void UI::queue(const sf::Drawable* s)
{
	m_sprites.push(s);
}

void UI::render(sf::RenderTexture& rt)
{
	// Clear the UI with a transparent color
	rt.clear(sf::Color(0,0,0,0));

	// Draw all drawables in the queue
	while (!m_sprites.empty())
	{
		rt.draw(*m_sprites.front());
		m_sprites.pop();
	}

	// Display is required in order for the rendertarget to render properly later
	rt.display();
}