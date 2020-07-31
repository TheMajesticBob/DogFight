#pragma once
#include <ecm.h>
#include <SFML/Audio.hpp>

class AmbientManager : public Entity 
{
public:
	AmbientManager() = delete;
	AmbientManager(class Scene* const s);

	void play()
	{
		for (auto layer : _musicLayers)
		{
			layer->play();
		}
	}

	void stop()
	{
		for (auto layer : _musicLayers)
		{
			layer->stop();
		}
	}

	void setGlobalVolume(float volume) 
	{ 
		_volumeMultiplier = volume; 

	}

	void setVolume(int layerId, float volume)
	{
		_musicLayers[layerId]->setVolume(volume);
	}

	void loadLayer(int layerId, std::string soundName);

	void setLayerNumber(int layerNum)
	{
		_musicLayers.resize(layerNum);
	}

protected:
	std::vector<std::shared_ptr<sf::Music>> _musicLayers;
	float _volumeMultiplier;
};