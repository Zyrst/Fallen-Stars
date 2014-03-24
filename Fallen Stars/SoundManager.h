#pragma once

#include <SFML/Audio/Sound.hpp>
#include <vector>

#include "ResourceCollection.h"

class SoundManager
{
public:
	static void playSound(sf::Sound sound);
	static void clear();

private:
	static SoundManager& instance();

	void cleanup();
	void internalPlaySound(sf::Sound& sound);
	void internalClear();

	std::vector<sf::Sound> sounds;
};

