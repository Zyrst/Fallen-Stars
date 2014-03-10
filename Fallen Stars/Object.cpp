#include "Object.h"
#include "SpriteSheet.h"
#include <Box2D\Box2D.h>
#include "BoxWorld.h"
#include "VecConverter.h"
#include <iostream>
#include <time.h>
#include "StatManager.h"


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

Object::Object(BoxWorld* world, sf::Vector2f& position, ResourceCollection& resource, TYPE type, StatManager* stats) :
	Entity(world,sf::Vector2f(100, 100),position),
	mResource(resource),
	mType(type),
	starCallBack(new StarCallBack(body->GetFixtureList())),
	mStats(stats)
{

	mStarDustSound = mResource.getSound("Assets/Sound/PickUp.wav");
	mStarSound = mResource.getSound("Assets/Sound/StarPickUp.wav");

	
	if(mType == TYPE::STAR)
	{

		body->GetFixtureList()->SetSensor(true);
		body->SetGravityScale(0.0f);

		b2Filter objectFilter = body->GetFixtureList()->GetFilterData();
		objectFilter.categoryBits = OBJECT;
		objectFilter.maskBits = PLAYER;
		body->GetFixtureList()->SetFilterData(objectFilter);

		/*Star animation*/
		auto &star = mResource.getTexture("Assets/Characters/Star anime.png");
		sf::Vector2i starSize = static_cast<sf::Vector2i>(star.getSize());
		sf::Vector2i frameSize(85,85); /* Alter to right size */

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
		auto &starDust = mResource.getTexture("Assets/Characters/StarDustAnimation.png");

		sf::Vector2i starDustSize = static_cast<sf::Vector2i>(starDust.getSize());
		sf::Vector2i frameSize(85,85); /* Alter to right size */
		SpriteSheet starDustSheet(frameSize, starDustSize);
		std::vector<sf::IntRect> starDustFrames = starDustSheet.getAllFrames();

		mStarDust = new Animation(starDustFrames, starDust);
		anime.setAnimation(*mStarDust);
		updateSpriteOrigin(SpriteOrigin::ORIGIN_CENTER, SpriteOrigin::ORIGIN_CENTER);
	}

	if(mType == TYPE::WINDOW)
	{
		/*Window animation, window anime.png is placeholdername*/
		auto &window = mResource.getTexture("Assets/Prop/Window anime.png");

		setupSensor(position);
		body->SetGravityScale(0.0f);
		body->SetFixedRotation(true);
		body->GetFixtureList()->SetSensor(true);

		sf::Vector2i windowSize = static_cast<sf::Vector2i>(window.getSize());
		sf::Vector2i frameSize(400, 287); /*Alter to right size if doesn't work*/
		SpriteSheet windowSheet (frameSize, windowSize);
		std::vector<sf::IntRect> windowFrames = windowSheet.getAllFrames();

		mWindow = new Animation(windowFrames, window);
		anime.setLooped(false);
		anime.setAnimation(*mWindow);
		updateSpriteOrigin(SpriteOrigin::ORIGIN_CENTER, SpriteOrigin::ORIGIN_CENTER);
	}
}

Object::~Object()
{
	delete mStar;
	delete mStarDust;
	delete mWindow;
}
	
void Object::update(sf::Time deltaTime)
{
	anime.setPosition(Convert::b2ToSfml(body->GetPosition()));
	anime.update(deltaTime);
	if (isAlive() && starCallBack->isColliding() && getType() == STARDUST)
	{
		playSound(STARDUST);
		mAlive = false;
		mStats->stardust +=1;
		std::cout << "Current amount of StarDust: " << mStats->stardust << std::endl;
	}
	if (isAlive() && starCallBack->isColliding() && getType() == STAR)
	{
		playSound(STAR);
		mAlive = false;
		mStats->stars +=1;
		std::cout << "Current amounts of stars:" << mStats->stars << std::endl;
	}

	/*Random Window opening, 'cuz why not*/
	/*std::srand(time(NULL));
	int result = std::rand() % 20 +1;*/
	//std::cout << mClock.getElapsedTime().asSeconds() << std::endl;
	if (mClock.getElapsedTime().asSeconds() > 5.0f /*result % 4 == 0*/)
	{
		anime.play();
		mClock.restart();
		//Tanken är att window ska spela bara var 5;e sekund, beror på designers
		if (getType() == WINDOW && starCallBack->isColliding())
		{
		 auto player = starCallBack->getPlayer();
		 auto distance = player->getPosition().x - getPosition().x;
		 std::cout << distance << std::endl;
		 if (distance > 1)
		{
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

void Object::playSound(TYPE type)
{
	if (type == STARDUST)
	{
		mStarDustSound.play();
	}
	if (type == STAR)
	{
		mStarSound.play();
	}
}

void Object::setupSensor(sf::Vector2f position)
{
	b2Vec2 bodySize = Convert::sfmlToB2(sf::Vector2f(400.0f, 287.0f));

	b2Vec2 bodyPosRight;
	b2Vec2 bodyPosLeft;

	bodyPosLeft.x = position.x;
	bodyPosLeft.y = position.y;
	
	bodyPosRight.x = position.x;
	bodyPosRight.y = position.y;

	//Left shape
	b2PolygonShape shapeLeft;
	shapeLeft.SetAsBox(-0.2f, 0.2f, bodyPosLeft, 0);
	b2FixtureDef defLeft;
	defLeft.isSensor = true;
	defLeft.shape = &shapeLeft;
	b2Fixture* fixLeft = body->CreateFixture(&defLeft);

	//Right shape
	b2PolygonShape shapeRight;
	shapeRight.SetAsBox(0.2f, 0.2f, bodyPosRight, 0);
	b2FixtureDef defRight;
	defRight.isSensor = true;
	defRight.shape = &shapeRight;
	b2Fixture* fixRight = body->CreateFixture(&defRight);
	
}