#include "SpriteSheet.h"

#include <SFML/Graphics/Sprite.hpp>
#include <cassert>

SpriteSheet::SpriteSheet(sf::Vector2i spriteSize, sf::Vector2i spriteSheetSize):
	mSpriteSize(spriteSize),
	mTiles(sf::Vector2i(spriteSheetSize.x / spriteSize.x, spriteSheetSize.y / spriteSize.y))
{}

int SpriteSheet::getFrameCount()
{
	return mTiles.x * mTiles.y;
}

sf::IntRect SpriteSheet::getFrame(int index)
{
	assert(0 < index && index < getFrameCount());
	int xIndex = index % mTiles.x;
	int yIndex = index / mTiles.x;
	return sf::IntRect(xIndex * mSpriteSize.x, yIndex * mSpriteSize.y, mSpriteSize.x, mSpriteSize.y);
}

sf::IntRect SpriteSheet::getMirroredFrame(int index)
{
	assert(0 < index && index < getFrameCount());
	int xIndex = index % mTiles.x;
	int yIndex = index / mTiles.x;
	return sf::IntRect(xIndex * mSpriteSize.x +mSpriteSize.x, yIndex * mSpriteSize.y, -mSpriteSize.x, mSpriteSize.y);
}

std::vector<sf::IntRect> SpriteSheet::getFrames(int startIndex, int frameCount, bool mirrored)
{
	std::vector<sf::IntRect> frames(frameCount);
	for(int i=0; i<frameCount; i++)
	{
		frames.push_back(getFrame(startIndex + i, mirrored));
	}
	return frames;
}

std::vector<sf::IntRect> SpriteSheet::getAllFrames(bool mirrored)
{
	return getFrames(0, getFrameCount(), mirrored);
}

sf::IntRect SpriteSheet::getFrame(int index, bool mirrored)
{
	if(!mirrored)
	{
		return getFrame(index);
	}
	else
	{
		return getMirroredFrame(index);
	}
};