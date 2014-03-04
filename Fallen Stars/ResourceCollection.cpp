#include "ResourceCollection.h"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <thread>
#include <iostream>

ResourceCollection::ResourceCollection():
	renderThreadID(std::this_thread::get_id())	
{}

ResourceCollection::~ResourceCollection()
{
	for(auto it = mTextures.begin(); it != mTextures.end(); it++) delete it->second;
	for(auto it = mSoundBuffers.begin(); it != mSoundBuffers.end(); it++) delete it->second;
	for(auto it = mFonts.begin(); it != mFonts.end(); it++) delete it->second;
	for(auto it = mShaders.begin(); it != mShaders.end(); it++) delete it->second;
	mTextures.clear();
	mSoundBuffers.clear();
	mFonts.clear();
	mShaders.clear();
}

/* Texture loader */

void ResourceCollection::loadTexture(std::string filename)
{
	if( mTextures.find(filename) == mTextures.end())
	{
		mTextures[filename] = new sf::Texture();

		if(mTextures[filename]->loadFromFile(filename))
		{
			std::cout << "Texture " << filename << " loaded successfully" << std::endl;
		}

		if(std::this_thread::get_id() == renderThreadID)
		{
			std::cout << "The texture was loaded in the game loop! This freezes the thread while loading!" << std::endl;
		}
	}
}

sf::Texture& ResourceCollection::getTexture(std::string filename)
{
	loadTexture(filename);
	return *mTextures[filename];
}


/* Sound loader */

void ResourceCollection::loadSound(std::string filename)
{
	if( mSoundBuffers.find(filename) == mSoundBuffers.end() )
	{
		mSoundBuffers[filename] = new sf::SoundBuffer();
		
		if(mSoundBuffers[filename]->loadFromFile(filename))
		{
			std::cout << "Sound " << filename << " loaded successfully" << std::endl;
		}

		if(std::this_thread::get_id() == renderThreadID)
		{
			std::cout << "The sound was loaded in the game loop! This freezes the thread while loading!" << std::endl;
		}
	}
}

sf::Sound ResourceCollection::getSound(std::string filename)
{
	loadSound(filename);
	return sf::Sound(*mSoundBuffers[filename]);
}


/* Font loader */

void ResourceCollection::loadFont(std::string filename)
{
	if( mFonts.find(filename) == mFonts.end() )
	{
		mFonts[filename] = new sf::Font();

		if(mFonts[filename]->loadFromFile(filename))
		{
			std::cout << "Font " << filename << " loaded successfully" << std::endl;
		}

		if(std::this_thread::get_id() == renderThreadID)
		{
			std::cout << "The font was loaded in the game loop! This freezes the thread while loading!" << std::endl;
		}
	}
}

sf::Font& ResourceCollection::getFont(std::string filename)
{
	loadFont(filename);
	return *mFonts[filename];
}

/* Shader loader */

void ResourceCollection::loadShader(std::string filename, sf::Shader::Type type)
{
	if( mShaders.find(filename) == mShaders.end() )
	{
		mShaders[filename] = new sf::Shader();

		if(mShaders[filename]->loadFromFile(filename, type))
		{
			std::cout << "Shader " << filename << " loaded successfully" << std::endl;
		}

		if(std::this_thread::get_id() == renderThreadID)
		{
			std::cout << "The shader was loaded in the game loop! This freezes the thread while loading!" << std::endl;
		}
	}
}

void ResourceCollection::loadShader(std::string vertFilename, std::string fragFilename)
{
	if( mShaders.find(vertFilename + fragFilename) == mShaders.end() )
	{
		mShaders[vertFilename + fragFilename] = new sf::Shader();

		if(mShaders[vertFilename + fragFilename]->loadFromFile(vertFilename, fragFilename))
		{
			std::cout << "Shader " << vertFilename << " + " << fragFilename << " loaded successfully" << std::endl;
		}

		if(std::this_thread::get_id() == renderThreadID)
		{
			std::cout << "The shader was loaded in the game loop! This freezes the thread while loading!" << std::endl;
		}
	}
}

sf::Shader& ResourceCollection::getShader(std::string filename, sf::Shader::Type type)
{
	loadShader(filename, type);
	return *mShaders[filename];
}

sf::Shader& ResourceCollection::getShader(std::string vertFilename, std::string fragFilename)
{
	loadShader(vertFilename, fragFilename);
	return *mShaders[vertFilename + fragFilename];
}