#include "SoundManager.h"

#include <string>

using namespace sf;

void SoundManager::playSound(sf::Sound sound)
{
	// Get a reference to the singleton instance
	SoundManager& soundManager = instance();

	// Whenever we're adding a new sound, perform some cleanup so old sounds don't stay in the list forever.
	soundManager.cleanup();

	// Take over the ownership of the sound and set it playing
	soundManager.internalPlaySound(sound);
}

void SoundManager::clear()
{
	instance().internalClear();
}

SoundManager& SoundManager::instance()
{
	static SoundManager singleton;
	return singleton;
}

void SoundManager::cleanup()
{
	auto it = sounds.begin();
	while(it != sounds.end())
	{
		Sound& sound = *it;
		if(sound.getStatus() == Sound::Status::Stopped) it = sounds.erase(it);
		else it++;
	}
}

void SoundManager::internalPlaySound(sf::Sound& sound)
{
	sounds.push_back(sound);
	sounds.back().play();
}

void SoundManager::internalClear()
{
	auto it = sounds.begin();
	while(it != sounds.end())
	{
		Sound& sound = *it;
		sound.stop();
		it = sounds.erase(it);
	}
}