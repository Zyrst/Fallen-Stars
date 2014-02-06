#pragma once

#include <map>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

class ResourceCollection : public TextureCollection, SoundCollection
{};

namespace // Anonymous namespace, unavailable outside of this file
{
	class TextureCollection
	{
	public:
		void preloadTexture(std::string filename);
		sf::Texture& getTexture(std::string filename);

	private:
		std::map<std::string, sf::Texture> mTextures;
	};

	class SoundCollection
	{
	public:
		void preloadSound(std::string filename);
		sf::Sound& getSound(std::string filename);

	private:
		std::map<std::string, sf::SoundBuffer> mSoundBuffers;
	};
}