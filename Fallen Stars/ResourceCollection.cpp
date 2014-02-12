#include "ResourceCollection.h"

#include <iostream>

ResourceCollection::ResourceCollection():
	renderThread(std::this_thread::get_id())	
{}

/* Texture loader */

void ResourceCollection::loadTexture(std::string filename)
{
	if( mTextures.find(filename) == mTextures.end())
	{
		std::cout << "Loading texture " << filename << std::endl;

		if(std::this_thread::get_id() == renderThread)
		{
			std::cout << "Texture " << filename << " was not preloaded!" << std::endl;
		}

		if(!mTextures[filename].loadFromFile(filename))
		{
			std::cout << "Texture " << filename << " failed to load!" << std::endl;
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
	std::cout << "Loading sound " << filename << std::endl;

	if( mSoundBuffers.find(filename) == mSoundBuffers.end() )
	{
		if(std::this_thread::get_id() == renderThread)
		{
			std::cout << "Sound " << filename << " was not preloaded!" << std::endl;
		}

		if(!mSoundBuffers[filename].loadFromFile(filename))
		{
			std::cout << "Sound " << filename << " failed to load!" << std::endl;
		}
	}
}

sf::Sound ResourceCollection::getSound(std::string filename)
{
	loadSound(filename);
	return sf::Sound(mSoundBuffers[filename]);
}