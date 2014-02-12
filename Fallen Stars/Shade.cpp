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
Shade::Shade(sf::Texture& texture, BoxWorld* world, sf::Vector2f& size, sf::Vector2f& position)
: EntityLiving(world,size,position),
  mFace(LEFT)

{
	setupSensors(position,size);
}
Shade::~Shade()
{
}
void Shade::render(sf::RenderTarget& renderTarget)
{

	sf::FloatRect rect = anime.getGlobalBounds();
	sf::RectangleShape sh = sf::RectangleShape(sf::Vector2f(rect.width, rect.height));
	sh.setPosition(rect.left, rect.top);
	sh.setFillColor(sf::Color::Transparent);
	sh.setOutlineColor(sf::Color::Blue);
	sh.setOutlineThickness(1.0f);
}
void Shade::update(sf::Time deltaTime)
{
	const b2Vec2& vel = body->GetLinearVelocity();
	mFace = getFacing();

	if(mFace = LEFT)
	{
		if(chaseSensorLeft->isChasing())
		{
			body->SetLinearVelocity(b2Vec2(-10*1.5, vel.y));
		}
		else
		{
		body->SetLinearVelocity(b2Vec2(-10, vel.y));
		}
	}
	else if(mFace = RIGHT)
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

	if(!ledgeSensorLeft->isGrounded()){
		setFacing(RIGHT);
	}
	else if(!ledgeSensorRight->isGrounded()){
		setFacing(LEFT);
	}
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
	
	groundPosRight.x += bodySize.x;
	groundPosRight.y += bodySize.y;
	groundPosLeft.y += bodySize.y;
	bodyPos.x += bodySize.x/2;
	bodyPos.y += bodySize.y/2;





//V�nster ChaseSensor
	b2PolygonShape shapeLeft;
	shapeLeft.SetAsBox(-3.0f, 0.2f, bodyPos, 0);
	b2FixtureDef defLeft;
	defLeft.isSensor = true;
	defLeft.shape = &shapeLeft;
	b2Fixture* fixLeft = body->CreateFixture(&defLeft);
	chaseSensorLeft = new ChaseSensor(fixLeft);
	fixLeft->SetUserData(chaseSensorLeft);

//H�ger ChaseSensor
	b2PolygonShape shapeRight;
	shapeRight.SetAsBox(3.0f, 0.2f, bodyPos, 0);
	b2FixtureDef defRight;
	defRight.isSensor = true;
	defRight.shape = &shapeRight;
	b2Fixture* fixRight = body->CreateFixture(&defRight);
	chaseSensorRight = new ChaseSensor(fixRight);
	fixRight->SetUserData(chaseSensorRight);


//V�nster Ground sensor
	b2PolygonShape shapeGroundLeft;
	shapeGroundLeft.SetAsBox(-0.2f, -0.2f,groundPosLeft,0);
	b2FixtureDef defGroundLeft;
	defGroundLeft.isSensor = true;
	defGroundLeft.shape = &shapeGroundLeft;
	b2Fixture* groundFixLeft = body->CreateFixture(&defGroundLeft);
	groundFixLeft->SetUserData(ledgeSensorLeft);
	
//H�ger Ground sensor
	b2PolygonShape shapeGroundRight;
	shapeGroundLeft.SetAsBox(0.2f, -0.2f,groundPosRight,0);
	b2FixtureDef defGroundRight;
	defGroundRight.isSensor = true;
	defGroundRight.shape = &shapeGroundRight;
	b2Fixture* groundFixRight = body->CreateFixture(&defGroundRight);
	groundFixRight->SetUserData(ledgeSensorRight);

//�ndrar kategorier och maskar till de olika sensorerna
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