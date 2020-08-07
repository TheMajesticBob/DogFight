#include "utils.h"
#include "engine.h"

sf::Vector2f Utils::WorldToScreen(const sf::Vector2f& position)
{
	sf::RenderWindow& rw = Engine::GetWindow();
	const sf::View& mainView = rw.getView(); 

	float viewScale = mainView.getSize().x / rw.getSize().x;
	sf::Vector2f viewCenter = mainView.getCenter();
	sf::Vector2f screenCenter = (sf::Vector2f(rw.getSize()) / 2.0f);

	sf::Vector2f worldToScreenOffset = ((position - viewCenter) / viewScale + screenCenter);
	
	return worldToScreenOffset;
}

sf::Vector2f Utils::ScreenToWorld(const sf::Vector2f& position)
{
	sf::RenderWindow& rw = Engine::GetWindow();
	const sf::View& mainView = rw.getView();
	
	float viewScale = mainView.getSize().x / rw.getSize().x; 
	sf::Vector2f viewCenter = mainView.getCenter();
	sf::Vector2f screenCenter = (sf::Vector2f(rw.getSize()) / 2.0f);
	
	sf::Vector2f mouseToCenterOffset = (position - screenCenter) * viewScale;

	return viewCenter + mouseToCenterOffset;
}