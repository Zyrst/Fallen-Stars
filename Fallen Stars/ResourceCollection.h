#pragma once

#include <map>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <thread>

/* Resource collections represent a loader that keeps resources inside of states.
 * States are expected to preload resources so that loading can be done while the
 * loading screen is displayed and while it is possible to skip the preloading, 
 * there will be warnings. */
class ResourceCollection
{
	public:
		ResourceCollection();

		// Textures
		void loadTexture(std::string filename);
		sf::Texture& getTexture(std::string filename);

		// Sounds
		void loadSound(std::string filename);
		sf::Sound getSound(std::string filename);

	private:
		/* Keep the id of the render thread, so a warning can be shown if resource
		 * loading is locking up the rendering */
		std::thread::id renderThread; 
		
		std::map<std::string, sf::Texture> mTextures;
		std::map<std::string, sf::SoundBuffer> mSoundBuffers;
};
