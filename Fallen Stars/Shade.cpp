#include "Shade.h"
#include <iostream>
#include <Box2D\Box2D.h>
#include <SFML\Graphics.hpp>
#include "CallBack.h"
#include "VecConverter.h"
#include "BoxWorld.h"
#include "ControlMapping.h"
#include "SpriteSheet.h"
#pragma region Chase
ChaseSensor::ChaseSensor(b2Fixture* owner)
	: CallBack(owner)
	, chasing(false)
	, mActive(false)
	, chaseVictim(nullptr)

{ }
void ChaseSensor::beginContact(b2Fixture* otherFixture)
{
	sf::FloatRect bounds = BoxBounds::boundsOfFixture(otherFixture);
	if (otherFixture->GetBody()->GetType() == b2_dynamicBody && chaseVictim == nullptr)
	{
		setVictim(otherFixture, bounds);
		//std::cout<<"Contact Begun"<<std::endl;
	}
	
}
void ChaseSensor::endContact(b2Fixture* otherFixture)
{
	if (otherFixture->GetBody()->GetType() == b2_dynamicBody &&  chaseVictim != nullptr)
	{
		chasing = false;
		chaseVictim = nullptr;
		//std::cout<<"Contact Begun Ended"<<std::endl;
	}
	
}
bool ChaseSensor::isChasing() const
{
	return (chaseVictim !=nullptr);
}
bool ChaseSensor::isActive() const
{
	return mActive;
}
void ChaseSensor::setActive(bool active)
{
	mActive = active;
}
void ChaseSensor::setVictim(b2Fixture* fix, const sf::FloatRect& bounds)
{
	chaseVictim = fix;
	victimBounds = sf::FloatRect(bounds);
}
#pragma endregion
#pragma region Ledge
LedgeSensor::LedgeSensor(b2Fixture* owner)
	: CallBack(owner)
	, groundFinder(nullptr)
{}
void LedgeSensor::beginContact(b2Fixture* otherFixture)
{
	sf::FloatRect bounds = BoxBounds::boundsOfFixture(otherFixture);
	if (otherFixture->GetBody()->GetType() == b2_staticBody && groundFinder == nullptr)
	{
		setFinder(otherFixture, bounds);
		//std::cout<<"G";
	}
}
void LedgeSensor::endContact(b2Fixture* otherFixture)
{
	if (otherFixture->GetBody()->GetType() == b2_staticBody && groundFinder != nullptr)
	{
		groundFinder = nullptr;
		//std::cout<<"420 ground not found lel"<<std::endl;
	}
}
bool LedgeSensor::isGrounded() const
{
	return (groundFinder != nullptr);
}
void LedgeSensor::setFinder(b2Fixture* fix, const sf::FloatRect& bounds)
{
	groundFinder = fix;
	finderBounds = sf::FloatRect(bounds);
}
#pragma endregion
#pragma region Shade
Shade::Shade(ResourceCollection& resource, BoxWorld* world, sf::Vector2f& size, sf::Vector2f& position)
: EntityLiving(world,size,position),
  mResource(resource)
{
	setMode(PATROL);
	
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
	
	anime.setAnimation(*mWalking);
	updateSpriteOrigin();
	setupSensors(position,size);
	setFacing(LEFT);
	b2Filter filter = (body->GetFixtureList())->GetFilterData();
	filter.categoryBits = ENEMY;
	filter.groupIndex = ALL, PLAYER;
	body->GetFixtureList()->SetFilterData(filter);
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
	if(currentMode == PATROL){
	const b2Vec2& vel = body->GetLinearVelocity();
	if(getFacing() == LEFT)
	{
		if(chaseSensorLeft->isChasing())
		{
			body->SetLinearVelocity(b2Vec2(-0.5*3.5, vel.y));
		}
		else if (!chaseSensorLeft->isChasing())
		{
			body->SetLinearVelocity(b2Vec2(-0.5, vel.y));
		}
	}
	else if(getFacing() == RIGHT)
	{
		if(chaseSensorRight->isChasing())
		{
			body->SetLinearVelocity(b2Vec2(0.5*3.5, vel.y));
		}
		else
		{
			body->SetLinearVelocity(b2Vec2(0.5, vel.y));
		}
	}
	
	
	if(!ledgeSensorLeft->isGrounded())
	{
		setFacing(RIGHT);
	}
	else if(!ledgeSensorRight->isGrounded())
	{
		setFacing(LEFT);
	}
	}
	else if(currentMode==ATTACK)
	{
		attack();
	}
	else if(currentMode==SPAWN)
	{
	}
	anime.update(deltaTime);
}
void Shade::setVelocityX(float x)
{
	velocity.x = x;
}
void Shade::setFacing(Facing face)
{
	Entity::setFacing(face);
	if(face == LEFT)
	{
		chaseSensorLeft->setActive(true);
		chaseSensorRight->setActive(false);
	}
	else if(face == RIGHT)
	{ 
		chaseSensorLeft->setActive(false);
		chaseSensorRight->setActive(true);
	}
}
void Shade::setupSensors(sf::Vector2f position, sf::Vector2f size)
{
	b2Vec2 bodySize = Convert::sfmlToB2(size);
	b2Vec2 bodyPosRight;
	b2Vec2 bodyPosLeft;
	b2Vec2 groundPosLeft; 
	b2Vec2 groundPosRight;
	
	groundPosRight.x = bodySize.x;
	groundPosRight.y = bodySize.y;
	groundPosLeft.y = bodySize.y;
	groundPosLeft.x = bodySize.x-(bodySize.x*2.0f);
	bodyPosLeft.x = bodySize.x-(bodySize.x*3.0f);
	bodyPosLeft.y = bodySize.y/2;
	bodyPosRight.y = bodySize.y/2;
	bodyPosRight.x = bodySize.x+(bodySize.x*1.2f);

//Vänster ChaseSensor
	b2PolygonShape shapeLeft;
	shapeLeft.SetAsBox(-1.0f, 0.05f, bodyPosLeft, 0);
	b2FixtureDef defLeft;
	defLeft.isSensor = true;
	defLeft.shape = &shapeLeft;
	b2Fixture* fixLeft = body->CreateFixture(&defLeft);
	chaseSensorLeft = new ChaseSensor(fixLeft);
	fixLeft->SetUserData(chaseSensorLeft);

//Höger ChaseSensor
	b2PolygonShape shapeRight;
	shapeRight.SetAsBox(1.0f, 0.05f, bodyPosRight, 0);
	b2FixtureDef defRight;
	defRight.isSensor = true;
	defRight.shape = &shapeRight;
	b2Fixture* fixRight = body->CreateFixture(&defRight);
	chaseSensorRight = new ChaseSensor(fixRight);
	fixRight->SetUserData(chaseSensorRight);


//Vänster Ground sensor
	b2PolygonShape shapeGroundLeft;
	shapeGroundLeft.SetAsBox(-0.05f, -0.15f,groundPosLeft,0);
	b2FixtureDef defGroundLeft;
	defGroundLeft.isSensor = true;
	defGroundLeft.shape = &shapeGroundLeft;
	b2Fixture* groundFixLeft = body->CreateFixture(&defGroundLeft);
	ledgeSensorLeft = new LedgeSensor(groundFixLeft);
	groundFixLeft->SetUserData(ledgeSensorLeft);
	
//Höger Ground sensor
	b2PolygonShape shapeGroundRight;
	shapeGroundRight.SetAsBox(0.05f, -0.15f,groundPosRight,0);
	b2FixtureDef defGroundRight;
	defGroundRight.isSensor = true;
	defGroundRight.shape = &shapeGroundRight;
	b2Fixture* groundFixRight = body->CreateFixture(&defGroundRight);
	ledgeSensorRight = new LedgeSensor(groundFixRight);
	groundFixRight->SetUserData(ledgeSensorRight);

//Ändrar kategorier och maskar till de olika sensorerna
	b2Filter filterGroundLeft = groundFixLeft->GetFilterData();
	b2Filter filterGroundRight = groundFixRight->GetFilterData();
	b2Filter filterChaseLeft = fixLeft->GetFilterData();
	b2Filter filterChaseRight = fixRight->GetFilterData();

	filterGroundLeft.categoryBits = ENEMY_GROUND;
	//filterGroundLeft.maskBits = ALL;
	groundFixLeft->SetFilterData(filterGroundLeft);

	b2Filter filterGroundRight = groundFixRight->GetFilterData();
	filterGroundRight.categoryBits = ENEMY_GROUND;
	//filterGroundRight.maskBits = ALL;
	groundFixRight->SetFilterData(filterGroundRight);

	b2Filter filterChaseLeft = fixLeft->GetFilterData();
	filterChaseLeft.categoryBits = ENEMY_CHASE;
	filterChaseLeft.maskBits = PLAYER;
	fixLeft->SetFilterData(filterChaseLeft);

	b2Filter filterChaseRight = fixRight->GetFilterData();
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
	//anime.setPosition(Convert::b2ToSfml(body->GetPosition()));

}
Shade::Mode Shade::getMode()
{
	return currentMode;
}
void Shade::setMode(Mode mode)
{
	currentMode = mode;
}
void Shade::attack()
{

}
/* Animationer , gå , springa, attackera , dö */
#pragma endregion
