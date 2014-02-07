#include "ResourceCollection.h"

#include <iostream>

void TextureCollection::preloadTexture(std::string filename)
{
	if( mTextures.find(filename) == mTextures.end() )
	{
		mTextures[filename].loadFromFile(filename);
	}
}

sf::Texture& TextureCollection::getTexture(std::string filename)
{
	if( mTextures.find(filename) == mTextures.end() )
	{
		std::cout << "Texture " << filename << " was not preloaded!" << std::endl;
		mTextures[filename].loadFromFile(filename);
	}
	return mTextures[filename];
}
/*
void SoundCollection::preloadSound(std::string filename)
{
	if( mSoundBuffers.find(filename) == mSoundBuffers.end() )
	{
		mSoundBuffers[filename].loadFromFile(filename);
	}
}

sf::Sound SoundCollection::getSound(std::string filename)
{
	if( mSoundBuffers.find(filename) == mSoundBuffers.end() )
	{
		std::cout << "Sound " << filename << " was not preloaded!" << std::endl;
		mSoundBuffers[filename].loadFromFile(filename);
	}
	return sf::Sound(mSoundBuffers[filename]);
}*/