#include "Object.h"
#include "SpriteSheet.h"
#include <Box2D\Box2D.h>
#include "BoxWorld.h"
#include "VecConverter.h"
#include "SoundManager.h"
#include "Game.h"
#include "Player.h"
#include "StatManager.h"
#include "ResourceCollection.h"


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

Object::Object(BoxWorld* world, sf::Vector2f size , sf::Vector2f& position, ResourceCollection* resource, TYPE type, StatManager* stats) :
	Entity(world,size,position),
	mResource(resource),
	mType(type),
	starCallBack(new StarCallBack(body->GetFixtureList())),
	mStats(stats)
{
	mStarDustSound = new sf::Sound;
	mStarDustSound->setBuffer(*mResource->getSound("Assets/Sound/PickUp.wav"));
	mStarSound = new sf::Sound;
	mStarSound->setBuffer(*mResource->getSound("Assets/Sound/StarPickUp.wav"));
	
	if(mType == TYPE::STAR)
	{

		body->GetFixtureList()->SetSensor(true);
		body->SetGravityScale(0.0f);

		b2Filter objectFilter = body->GetFixtureList()->GetFilterData();
		objectFilter.categoryBits = OBJECT;
		objectFilter.maskBits = PLAYER;
		body->GetFixtureList()->SetFilterData(objectFilter);

		/*Star animation*/
		auto &star = mResource->getTexture("Assets/Characters/Star anime.png");
		sf::Vector2i starSize = static_cast<sf::Vector2i>(star.getSize());
		sf::Vector2i frameSize(85,85);

		SpriteSheet starSheet(frameSize, starSize);
		std::vector<sf::IntRect> starFrames = starSheet.getAllFrames();

		mStar = new Animation(starFrames, star);
		anime.setAnimation(*mStar);
		updateSpriteOrigin(SpriteOrigin::ORIGIN_CENTER, SpriteOrigin::ORIGIN_CENTER);
		

	}

	if (mType == TYPE::STARDUST)
	{
		body->GetFixtureList()->SetSensor(true);
		body->SetGravityScale(0.0f);

		b2Filter objectFilter = body->GetFixtureList()->GetFilterData();
		objectFilter.categoryBits = OBJECT;
		objectFilter.maskBits = PLAYER;
		body->GetFixtureList()->SetFilterData(objectFilter);

		/*StarDust animation*/
		auto &starDust = mResource->getTexture("Assets/Characters/StarDustAnimation.png");

		sf::Vector2i starDustSize = static_cast<sf::Vector2i>(starDust.getSize());
		sf::Vector2i frameSize(85,85);
		SpriteSheet starDustSheet(frameSize, starDustSize);
		std::vector<sf::IntRect> starDustFrames = starDustSheet.getAllFrames();

		mStarDust = new Animation(starDustFrames, starDust);
		anime.setAnimation(*mStarDust);
		updateSpriteOrigin(SpriteOrigin::ORIGIN_CENTER, SpriteOrigin::ORIGIN_CENTER);
	}

	if(mType == TYPE::WINDOW)
	{
		/*Window animation, window anime.png is placeholdername*/
		auto &window = mResource->getTexture("Assets/Prop/Window anime.png");
		sf::Vector2i windowSize = static_cast<sf::Vector2i>(window.getSize());
		sf::Vector2i frameSize(400, 287);
		SpriteSheet windowSheet (frameSize, windowSize);
		std::vector<sf::IntRect> windowFrames = windowSheet.getAllFrames();

		b2Filter objectFilter = body->GetFixtureList()->GetFilterData();
		objectFilter.categoryBits = OBJECT;
		objectFilter.maskBits = PLAYER;
		body->GetFixtureList()->SetFilterData(objectFilter);

		mWindow = new Animation(windowFrames, window);
		anime.setLooped(false);
		anime.setAnimation(*mWindow);
		body->GetFixtureList()->SetSensor(true);
		body->SetGravityScale(0.0f);
		body->SetFixedRotation(true);
		updateSpriteOrigin(SpriteOrigin::ORIGIN_CENTER, SpriteOrigin::ORIGIN_CENTER);
	}
}

Object::~Object()
{
	delete mStar;
	delete mStarDust;
	delete mWindow;
	delete mStarDustSound;
	delete mStarSound;
}
	
void Object::update(sf::Time deltaTime)
{

	anime.setPosition(Convert::b2ToSfml(body->GetPosition()));
	anime.update(deltaTime);
	if (isAlive() && starCallBack->isColliding() && getType() == STARDUST)
	{
		if (mStats->stardust == false)
		{
		SoundManager::playSound(*mStarDustSound);	
		mStats->stardust = true;
		}
		
	}
	if (isAlive() && starCallBack->isColliding() && getType() == STAR)
	{
		SoundManager::playSound(*mStarSound);
		mAlive = false;
		mStats->stars +=1;
	}

	if (mClock.getElapsedTime().asSeconds() > 5.0f)
	{
		anime.play();
		mClock.restart();
		if (getType() == WINDOW && starCallBack->isColliding())
		{
			Player* player = starCallBack->getPlayer();
			float distance = player->getPosition().x - getPosition().x;
			if (distance > 28)
			{
				//Push to the right
				player->setState(Player::KNOCKEDBACKED);
				player->getBody()->ApplyForce(b2Vec2(100,0),b2Vec2(0,0),true);

			}
			if (distance < 28)
			{
				//Push to left
				player->setState(Player::KNOCKEDBACKED);
				player->getBody()->ApplyForce(b2Vec2(-100,0),b2Vec2(0,0),true);
			}
		}
	
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