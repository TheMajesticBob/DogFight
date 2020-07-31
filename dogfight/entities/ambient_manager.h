#pragma once
#include <ecm.h>
#include <map>
#include <SFML/Audio.hpp>

class AmbientManager : public Entity 
{
public:
	AmbientManager() = delete;
	AmbientManager(class Scene* const s);

	~AmbientManager()
	{
	}

	void play(std::string layerId)
	{
		_musicLayers[layerId]->play();
	}

	void stop(std::string layerId)
	{
		_musicLayers[layerId]->stop();
	}

	void setGlobalVolume(float volume)
	{ 
		_volumeMultiplier = volume; 

	}

	void setVolume(std::string layerId, float volume)
	{
		_musicLayers[layerId]->setVolume(volume);
	}

	void loadLayer(std::string layerId, std::string soundName);

protected:
	std::map<std::string, std::shared_ptr<sf::Music>> _musicLayers;
	float _volumeMultiplier;
};