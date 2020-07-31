//"cmp_audio.h"
#pragma once
#include <ecm.h>
#include <SFML/Audio.hpp>
#include <system_resources.h>

// A component to allow basic movement behaviour
class AudioComponent : public Component {
public:
	enum Type
	{
		Effect,
		Music
	};

protected:
	std::shared_ptr<sf::SoundBuffer> _soundBuffer;
	std::shared_ptr<sf::Sound> _sound;
	float _volume;
	Type _type;

	void setVolumeInternal();

public:

	void loadSound(std::string name);
	void playSound();
	void setVolume(float value)
	{
		_volume = value;
		setVolumeInternal();
	}

	void setSoundType(Type type)
	{
		_type = type;
	}

	std::shared_ptr<sf::Sound> getSound() { return _sound; }

	// Will check the keyboard and move the component's parent.
	void update(double) override;
	// Component does not need rendered.
	void render() override {}
	// Used to create the component for an entity
	explicit AudioComponent(Entity* p);

	AudioComponent() = delete;
};