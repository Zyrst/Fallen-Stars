#include "ResourceCollection.h"

#include <iostream>

ResourceCollection::ResourceCollection():
	renderThread(std::this_thread::get_id())	
{}

ResourceCollection::~ResourceCollection()
{
	for(auto it = mShaders.begin(); it != mShaders.end(); it++) 
	{
		delete it->second;
	}
}

/* Texture loader */

void ResourceCollection::loadTexture(std::string filename)
{
	if( mTextures.find(filename) == mTextures.end())
	{
		if(mTextures[filename].loadFromFile(filename))
		{
			std::cout << "Texture " << filename << " loaded successfully" << std::endl;
		}

		if(std::this_thread::get_id() == renderThread)
		{
			std::cout << "The texture was loaded in the game loop! This freezes the thread while loading!" << std::endl;
		}
	}
}

sf::Texture& ResourceCollection::getTexture(std::string filename)
{
	loadTexture(filename);
	return mTextures[filename];
}


/* Sound loader */

void ResourceCollection::loadSound(std::string filename)
{
	if( mSoundBuffers.find(filename) == mSoundBuffers.end() )
	{
		if(mSoundBuffers[filename].loadFromFile(filename))
		{
			std::cout << "Sound " << filename << " loaded successfully" << std::endl;
		}

		if(std::this_thread::get_id() == renderThread)
		{
			std::cout << "The sound was loaded in the game loop! This freezes the thread while loading!" << std::endl;
		}
	}
}

sf::Sound ResourceCollection::getSound(std::string filename)
{
	loadSound(filename);
	return sf::Sound(mSoundBuffers[filename]);
}


/* Font loader */

void ResourceCollection::loadFont(std::string filename)
{
	if( mFonts.find(filename) == mFonts.end() )
	{
		if(mFonts[filename].loadFromFile(filename))
		{
			std::cout << "Font " << filename << " loaded successfully" << std::endl;
		}

		if(std::this_thread::get_id() == renderThread)
		{
			std::cout << "The font was loaded in the game loop! This freezes the thread while loading!" << std::endl;
		}
	}
}

sf::Font& ResourceCollection::getFont(std::string filename)
{
	loadFont(filename);
	return mFonts[filename];
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

		if(std::this_thread::get_id() == renderThread)
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
