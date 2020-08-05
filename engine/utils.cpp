#include "utils.h"
#include "engine.h"

sf::Vector2f Utils::WorldToScreen(const sf::Vector2f& position)
{
	sf::RenderWindow& rw = Engine::GetWindow();
	const sf::View& mainView = rw.getView(); 
	
	sf::Vector2f viewCenter = mainView.getCenter();
	sf::Vector2f viewPosition = sf::Vector2f((viewCenter + mainView.getSize() / 2.0f).x, (viewCenter + mainView.getSize() / 2.0f).y);
	sf::Vector2f viewScale = sf::Vector2f(mainView.getSize().x / rw.getSize().x, -mainView.getSize().y / rw.getSize().y);

	return (viewPosition - (viewCenter*2.0f) + position) / viewScale.x;
}

sf::Vector2f Utils::ScreenToWorld(const sf::Vector2f& position)
{
	sf::RenderWindow& rw = Engine::GetWindow();
	const sf::View& mainView = rw.getView();

	sf::Vector2f viewCenter = mainView.getCenter();
	sf::Vector2f viewPosition = sf::Vector2f((viewCenter + mainView.getSize() / 2.0f).x, (viewCenter + mainView.getSize() / 2.0f).y);
	sf::Vector2f viewScale = sf::Vector2f(mainView.getSize().x / rw.getSize().x, -mainView.getSize().y / rw.getSize().y);

	return (position - (viewPosition - viewCenter * 2.0f)) * viewScale.x;
}