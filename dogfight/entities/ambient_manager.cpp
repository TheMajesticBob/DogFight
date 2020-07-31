#include "ambient_manager.h"
#include "../components/cmp_audio.h"

AmbientManager::AmbientManager(class Scene* const s)
	: Entity(s)
{
}

void AmbientManager::loadLayer(int layerId, std::string soundName)
{
	_musicLayers[layerId] = Resources::get<sf::Music>(soundName);
}
