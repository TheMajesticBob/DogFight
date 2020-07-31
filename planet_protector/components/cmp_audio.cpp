//"cmp_basic_movement.cpp"
#include "cmp_audio.h"
#include <system_resources.h>
#include "../planet_protector/game.h"

void AudioComponent::setVolumeInternal()
{
	float volumeMultiplier = 1.0f;
	switch (_type)
	{
		case Type::Effect:
		default:
			volumeMultiplier = Audio::EffectsVolume;
			break;

		case Type::Music:
			volumeMultiplier = Audio::MusicVolume;
			break;
	}

	_sound->setVolume(_volume * volumeMultiplier * Audio::MasterVolume);
}

void AudioComponent::loadSound(std::string name)
{
	_soundBuffer = Resources::get<sf::SoundBuffer>(name);
	_sound->setBuffer(*_soundBuffer.get());
}

void AudioComponent::playSound()
{
	if (_soundBuffer)
	{
		_sound->play();
	}
}

void AudioComponent::update(double)
{

}

AudioComponent::AudioComponent(Entity* p) : Component(p)
{
	_sound = std::make_shared<sf::Sound>();
}
