#include "Object.h"
#include "SpriteSheet.h"

Object::Object(BoxWorld* world, sf::Vector2f& position, ResourceCollection& resource, TYPE type) :
	Entity(world,sf::Vector2f(20,20),position),
	mResource(resource),
	mType(type)
{
	if(mType == TYPE::STAR)
	{
		/*Star animation*/
		auto &star = mResource.getTexture("Assets/Map/trashcanfull.png");

		sf::Vector2i starSize = static_cast<sf::Vector2i>(star.getSize());
		sf::Vector2i frameSize(256,256); /* Alter to right size */

		SpriteSheet starSheet(frameSize, starSize);
		std::vector<sf::IntRect> starFrames = starSheet.getAllFrames();

		mStar = new Animation(starFrames, star);
		
	}

	if (mType == TYPE::STARDUST)
	{
		/*StarDust animation*/
		/*auto &starDust = mResource.getTexture("Namn på fil.png");

		sf::Vector2i starDustSize = static_cast<sf::Vector2i>(starDust.getSize());
		SpriteSheet starDustSheet(frameSize, starDustSize);
		std::vector<sf::IntRect> starDustFrames = starDustSheet.getAllFrames();

		mStarDust = new Animation(starDustFrames, starDust);
		*/
	}
}
	
void Object::update(sf::Time deltaTime)
{
	anime.update(deltaTime);
}

Object::TYPE Object::getType()
{
	return mType;
}

void Object::handleAction(Controls::Action action, Controls::KeyState)
{
	
}

void Object::render(sf::RenderTarget& target)
{
	Entity::render(target);
}


/* Collision stuff, returns points to starcount and stardust
* on collision*/