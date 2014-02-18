#include "Shade.h"
#include <iostream>
#include <Box2D\Box2D.h>
#include <SFML\Graphics.hpp>
#include "CallBack.h"
#include "VecConverter.h"
#include "BoxWorld.h"
#include "ControlMapping.h"
#include "SpriteSheet.h"


ChaseSensor::ChaseSensor(b2Fixture* owner)
	: CallBack(owner)
	, chasing(0)
	, mActive(false)
{ }
void ChaseSensor::beginContact(b2Fixture* otherFixture)
{
	if (otherFixture->GetBody()->GetType() == b2_kinematicBody && isActive()==true)
	{
		chasing++;
	}
}
void ChaseSensor::endContact(b2Fixture* otherFixture)
{
	if (otherFixture->GetBody()->GetType() == b2_kinematicBody && isActive() == false)
	{
		chasing--;
	}
}
bool ChaseSensor::isChasing()
{
	return (chasing > 0);
}
bool ChaseSensor::isActive()
{
	return mActive;
}
void ChaseSensor::setActive(bool active)
{
	mActive = active;
}
/*----------------------------*/
LedgeSensor::LedgeSensor(b2Fixture* otherFixture)
	: CallBack(owner)
	, grounded(0)
	, mActive(false)
{}
void LedgeSensor::beginContact(b2Fixture* otherFixture)
{
	if (otherFixture->GetBody()->GetType() == b2_staticBody)
	{
		mActive = true;
	}
}
void LedgeSensor::endContact(b2Fixture* otherFixture)
{
	if (otherFixture->GetBody()->GetType() == b2_staticBody)
	{
		mActive = false;
	}
}
bool LedgeSensor::isGrounded() const
{
	return mActive;
}
/*----------------------------*/
Shade::Shade(ResourceCollection& resource, BoxWorld* world, sf::Vector2f& size, sf::Vector2f& position)
: EntityLiving(world,size,position),
  mFace(LEFT),
  mResource(resource)
{
	setupSensors(position,size);

	auto &idle = mResource.getTexture("Assets/Characters/Shade_idle.png");
	sf::Vector2i idleSize = static_cast<sf::Vector2i>(idle.getSize());
	sf::Vector2i frameSize(256,256);
	SpriteSheet idleSheet(frameSize,idleSize);
	std::vector<sf::IntRect> idleFrames = idleSheet.getAllFrames();
	mIdle = new Animation(idleFrames, idle);

	auto &walking = mResource.getTexture("Assets/Characters/Shade_walking.png");
	sf::Vector2i walkSize = static_cast<sf::Vector2i>(walking.getSize());
	SpriteSheet walkSheet(frameSize,walkSize);
	std::vector<sf::IntRect> walkFrames = walkSheet.getAllFrames();
	mWalking = new Animation(walkFrames, walking);

	auto &spawn = mResource.getTexture("Assets/Characters/Shade_spawn.png");
	sf::Vector2i spawnSize = static_cast<sf::Vector2i>(spawn.getSize());
	SpriteSheet spawnSheet(frameSize,spawnSize);
	std::vector<sf::IntRect> spawnFrames = spawnSheet.getAllFrames();
	mSpawn = new Animation(spawnFrames,spawn);
	
	anime.setAnimation(*mSpawn);

	updateSpriteOrigin();
}
Shade::~Shade()
{
	delete mIdle;
	delete mWalking;
	delete mSpawn;
}
void Shade::render(sf::RenderTarget& renderTarget)
{

	sf::FloatRect rect = anime.getGlobalBounds();
	sf::RectangleShape sh = sf::RectangleShape(sf::Vector2f(rect.width, rect.height));
	sh.setPosition(rect.left, rect.top);
	sh.setFillColor(sf::Color::Transparent);
	sh.setOutlineColor(sf::Color::Blue);
	sh.setOutlineThickness(1.0f);
	Entity::render(renderTarget);
}
void Shade::update(sf::Time deltaTime)
{
	const b2Vec2& vel = body->GetLinearVelocity();
	mFace = getFacing();

	if(mFace == LEFT)
	{
		if(chaseSensorLeft->isChasing())
		{
			body->SetLinearVelocity(b2Vec2(-3*1.5, vel.y));
			std::cout << "I'm chasing left" << std::endl;
		}
		else if (!chaseSensorLeft->isChasing())
		{
			body->SetLinearVelocity(b2Vec2(-3, vel.y));
			std::cout << "I'm not chasing left" << std::endl;
		}
	}
	else if(mFace == RIGHT)
	{
		if(chaseSensorRight->isChasing())
		{
			body->SetLinearVelocity(b2Vec2(10*1.5, vel.y));
		}
		else
		{
			body->SetLinearVelocity(b2Vec2(10, vel.y));
		}
	}
	/*
	if(!ledgeSensorLeft->isGrounded()){
		setFacing(RIGHT);
	}
	else if(!ledgeSensorRight->isGrounded()){
		setFacing(LEFT);
	}*/

	anime.update(deltaTime);
}
void Shade::setVelocityX(float x)
{
	velocity.x = x;
}
void Shade::setFacing(Facing Face)
{
	mFace = Face;
	if(Face = LEFT)
	{
		chaseSensorLeft->setActive(true);
		chaseSensorRight->setActive(false);
	}
	else if(Face = RIGHT)
	{ 
		chaseSensorLeft->setActive(false);
		chaseSensorRight->setActive(true);
	}
}
void Shade::setupSensors(sf::Vector2f position, sf::Vector2f size)
{
	b2Vec2 bodyPos = Convert::sfmlToB2(position);
	b2Vec2 bodySize = Convert::sfmlToB2(size);
	b2Vec2 groundPosLeft = Convert::sfmlToB2(position);
	b2Vec2 groundPosRight = Convert::sfmlToB2(position);
	
	groundPosRight.x = bodySize.x;
	groundPosRight.y = bodySize.y;
	groundPosLeft.y = bodySize.y;
	groundPosLeft.x = bodySize.x;
	bodyPos.x = bodySize.x/2;
	bodyPos.y = bodySize.y/2;





//Vänster ChaseSensor
	b2PolygonShape shapeLeft;
	shapeLeft.SetAsBox(-3.0f, 0.2f, bodyPos, 0);
	b2FixtureDef defLeft;
	defLeft.isSensor = true;
	defLeft.shape = &shapeLeft;
	b2Fixture* fixLeft = body->CreateFixture(&defLeft);
	chaseSensorLeft = new ChaseSensor(fixLeft);
	fixLeft->SetUserData(chaseSensorLeft);

//Höger ChaseSensor
	b2PolygonShape shapeRight;
	shapeRight.SetAsBox(3.0f, 0.2f, bodyPos, 0);
	b2FixtureDef defRight;
	defRight.isSensor = true;
	defRight.shape = &shapeRight;
	b2Fixture* fixRight = body->CreateFixture(&defRight);
	chaseSensorRight = new ChaseSensor(fixRight);
	fixRight->SetUserData(chaseSensorRight);


//Vänster Ground sensor
	b2PolygonShape shapeGroundLeft;
	shapeGroundLeft.SetAsBox(-0.2f, -0.2f,groundPosLeft,0);
	b2FixtureDef defGroundLeft;
	defGroundLeft.isSensor = true;
	defGroundLeft.shape = &shapeGroundLeft;
	b2Fixture* groundFixLeft = body->CreateFixture(&defGroundLeft);
	groundFixLeft->SetUserData(ledgeSensorLeft);
	
//Höger Ground sensor
	b2PolygonShape shapeGroundRight;
	shapeGroundLeft.SetAsBox(0.2f, -0.2f,groundPosRight,0);
	b2FixtureDef defGroundRight;
	defGroundRight.isSensor = true;
	defGroundRight.shape = &shapeGroundRight;
	b2Fixture* groundFixRight = body->CreateFixture(&defGroundRight);
	groundFixRight->SetUserData(ledgeSensorRight);

//Ändrar kategorier och maskar till de olika sensorerna
	b2Filter filterGroundLeft = groundFixLeft->GetFilterData();
	b2Filter filterGroundRight = groundFixRight->GetFilterData();
	b2Filter filterChaseLeft = fixLeft->GetFilterData();
	b2Filter filterChaseRight = fixRight->GetFilterData();

	filterGroundLeft.categoryBits = ENEMY_GROUND;
	filterGroundLeft.maskBits = ALL;
	filterGroundRight.categoryBits = ENEMY_GROUND;
	filterGroundRight.maskBits = ALL;

	filterChaseLeft.categoryBits = ENEMY_CHASE;
	filterChaseLeft.maskBits = PLAYER;
	filterChaseRight.categoryBits = ENEMY_CHASE;
	filterChaseRight.maskBits = PLAYER;


	groundFixLeft->SetFilterData(filterGroundLeft);
	groundFixRight->SetFilterData(filterGroundRight);
	fixLeft->SetFilterData(filterChaseLeft);
	fixRight->SetFilterData(filterChaseRight);

}

void Shade::handleAction(Controls::Action action, Controls::KeyState)
{

}

void Shade::updateAnimation()
{
	Animation* currentAnimation = NULL;
	if(chaseSensorLeft->isActive() || chaseSensorRight->isActive())
	{
		currentAnimation = mWalking;
	}

	if(!chaseSensorLeft->isActive() && !chaseSensorRight->isActive())
	{
		currentAnimation = mIdle;
	}

	if (currentAnimation != NULL) anime.play(*currentAnimation);
	anime.setPosition(Convert::b2ToSfml(body->GetPosition()));
}

/* Animationer , gå , springa, attackera , dö */