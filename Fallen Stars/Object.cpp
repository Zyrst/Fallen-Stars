#include "Object.h"
#include "SpriteSheet.h"

Object::Object(BoxWorld* world, sf::Vector2f& position, ResourceCollection& resource, TYPE type) :
	Entity(world,sf::Vector2f(20,20),position),
	mResource(resource),
	mType(type)
{
	if(mType == TYPE::STAR)
	{
		/*auto &star = mResource.getTexture("Namn på fil.png");

		sf::Vector2i starSize = static_cast<sf::Vector2i>(star.getSize());
		sf::Vector2i frameSize(256,256); /* Altered to right size */
/*
		SpriteSheet spriteSheet(frameSize, starSize);
		std::vector<sf::IntRect> frames = spriteSheet.getAllFrames();

		mStar = new Animation(frames, star);
		*/
	}

	if (mType == TYPE::STARDUST)
	{
		/*Same as above*/
	}
}
	
void Object::update(sf::Time deltaTime)
{
	
}

bool Object::getType()
{
	return mType;
}

/* Collision stuff, returns points to starcount and stardust
* on collision*/