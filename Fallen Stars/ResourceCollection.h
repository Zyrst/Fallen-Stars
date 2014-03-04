#pragma once

#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <thread>
#include <map>

namespace sf { class Texture;
			   class SoundBuffer;
			   class Font; }

/* Resource collections represent a loader that keeps resources inside of states.
 * States are expected to load resources inside of the loading function in order
 * to be loaded in a separate loading thread. If the thread loads resources at
 * any other time, there will be a warning. */
class ResourceCollection
{
	public:
		ResourceCollection();
		~ResourceCollection();

		// Textures
		void loadTexture(std::string filename);
		sf::Texture& getTexture(std::string filename);

		// Sounds
		void loadSound(std::string filename);
		sf::Sound getSound(std::string filename);

		// Fonts
		void loadFont(std::string filename);
		sf::Font& getFont(std::string filename);

		// Shaders
		void loadShader(std::string filename, sf::Shader::Type);
		sf::Shader& getShader(std::string filename, sf::Shader::Type);
		void loadShader(std::string vertFilename, std::string fragFilename);
		sf::Shader& getShader(std::string vertFilename, std::string fragFilename);

	private:
		/* Keep the id of the render thread, so a warning can be shown if resource
		 * loading is locking up the rendering */
		std::thread::id renderThreadID; 
		
		std::map<std::string, sf::Texture*> mTextures;
		std::map<std::string, sf::SoundBuffer*> mSoundBuffers;
		std::map<std::string, sf::Font*> mFonts;
		std::map<std::string, sf::Shader*> mShaders;
};
