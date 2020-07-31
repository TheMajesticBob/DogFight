//"cmp_basic_movement.cpp"
#include "cmp_audio.h"
#include <system_resources.h>

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

void AudioComponent::setVolume(float value)
{
	_sound->setVolume(value);
}

void AudioComponent::update(double)
{

}

AudioComponent::AudioComponent(Entity* p) : Component(p)
{
	_sound = std::make_shared<sf::Sound>();
}
