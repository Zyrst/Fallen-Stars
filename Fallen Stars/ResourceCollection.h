#pragma once

#include <map>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>


	// Anonymous namespace, unavailable outside of this file
	class TextureCollection
	{
	public:
		void preloadTexture(std::string filename);
		sf::Texture& getTexture(std::string filename);

	private:
		std::map<std::string, sf::Texture> mTextures;
	};
	/*
	class SoundCollection
	{
	public:
		void preloadSound(std::string filename);
		sf::Sound getSound(std::string filename);

	private:
		std::map<std::string, sf::SoundBuffer> mSoundBuffers;
	};*/


/* Resource collections represent a loader that keeps resources inside of states.
 * States are expected to preload resources so that loading can be done while the
 * loading screen is displayed and while it is possible to skip the preloading, 
 * there will be warnings. */

class ResourceCollection : public TextureCollection//, SoundCollection
{};
