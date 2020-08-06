#include "utils.h"
#include "engine.h"

sf::Vector2f Utils::WorldToScreen(const sf::Vector2f& position)
{
	sf::RenderWindow& rw = Engine::GetWindow();
	const sf::View& mainView = rw.getView(); 

	sf::Vector2f viewCenter = mainView.getCenter();
	sf::Vector2f viewPosition = sf::Vector2f((viewCenter + mainView.getSize() / 2.0f).x, (viewCenter + mainView.getSize() / 2.0f).y);

	float viewScale = mainView.getSize().x / rw.getSize().x;

	return (viewPosition - (viewCenter*2.0f) + position) / viewScale;
}

sf::Vector2f Utils::ScreenToWorld(const sf::Vector2f& position)
{
	sf::RenderWindow& rw = Engine::GetWindow();
	const sf::View& mainView = rw.getView();
	
	float viewScale = mainView.getSize().x / rw.getSize().x; 
	sf::Vector2f viewCenter = mainView.getCenter();
	sf::Vector2f screenCenter = (sf::Vector2f(rw.getSize()) / 2.0f);
	
	sf::Vector2f mouseToCenterOffset = (position - screenCenter) * viewScale;

	return (viewCenter + mouseToCenterOffset);
}