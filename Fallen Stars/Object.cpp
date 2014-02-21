#include "Object.h"
#include "SpriteSheet.h"
#include <Box2D\Box2D.h>
#include "BoxWorld.h"
#include "VecConverter.h"
#include <iostream>


StarCallBack::StarCallBack(b2Fixture* owner)
	: CallBack(owner)
	, player(nullptr)
{

}

void StarCallBack::beginContact(b2Fixture* otherFixture)
{
	Entity* v = (Entity*) otherFixture->GetBody()->GetUserData();
	Player* p = dynamic_cast<Player*>(v);

	if (p != nullptr)
	{
		player = p;
	}
}
void StarCallBack::endContact(b2Fixture* otherFixture)
{
	Entity* v = (Entity*) otherFixture->GetBody()->GetUserData();
	Player* p = dynamic_cast<Player*>(v);
	
	if (p != nullptr && p == player)
	{
		player = nullptr;
	}
}

bool StarCallBack::isColliding() const
{
	return (player != nullptr);
}

Player* StarCallBack::getPlayer()
{
	return player;
}

Object::Object(BoxWorld* world, sf::Vector2f& position, ResourceCollection& resource, TYPE type) :
	Entity(world,sf::Vector2f(100, 100),position),
	mResource(resource),
	mType(type),
	starCallBack(new StarCallBack(body->GetFixtureList()))
{

	body->GetFixtureList()->SetSensor(true);
	body->SetGravityScale(0.0f);
	mStarDustSound = mResource.getSound("Assets/Sound/PickUp.wav");
	mStarSound = mResource.getSound("Assets/Sound/StarPickUp.wav");

	

	if(mType == TYPE::STAR)
	{
		/*Star animation*/
		auto &star = mResource.getTexture("Assets/Map/SmallBox.png");
		sf::Vector2i starSize = static_cast<sf::Vector2i>(star.getSize());
		sf::Vector2i frameSize(120,120); /* Alter to right size */

		SpriteSheet starSheet(frameSize, starSize);
		std::vector<sf::IntRect> starFrames = starSheet.getAllFrames();

		mStar = new Animation(starFrames, star);
		anime.setAnimation(*mStar);
		updateSpriteOrigin();
		

	}

	if (mType == TYPE::STARDUST)
	{
		/*StarDust animation*/
		auto &starDust = mResource.getTexture("Assets/Map/SmallBox.png");

		sf::Vector2i starDustSize = static_cast<sf::Vector2i>(starDust.getSize());
		sf::Vector2i frameSize(120,120); /* Alter to right size */
		SpriteSheet starDustSheet(frameSize, starDustSize);
		std::vector<sf::IntRect> starDustFrames = starDustSheet.getAllFrames();

		mStarDust = new Animation(starDustFrames, starDust);
		anime.setAnimation(*mStarDust);
		updateSpriteOrigin();
	}
}

Object::~Object()
{
	delete mStar;
	delete mStarDust;
}
	
void Object::update(sf::Time deltaTime)
{
	anime.setPosition(Convert::b2ToSfml(body->GetPosition()));
	anime.update(deltaTime);
	if (isAlive() && starCallBack->isColliding() && getType() == STARDUST)
	{
		playSound(STARDUST);
		std::cout << "Collect stardust wow :D" << std::endl;
		mAlive = false;
	}
	if (isAlive() && starCallBack->isColliding() && getType() == STAR)
	{
		playSound(STAR);
		std::cout<< "Do you even get stars" << std::endl;
		mAlive = false;
	}
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

void Object::playSound(TYPE type)
{
	if (type == STARDUST)
	{
		mStarDustSound.play();
		std::cout << "Do you even make a sound when you stardust" << std::endl;
	}
	if (type == STAR)
	{
		mStarSound.play();
		std::cout << "Star sounds are the best sounds" << std::endl;
	}
}