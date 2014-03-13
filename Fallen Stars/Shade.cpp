#include "Shade.h"
#include <iostream>
#include <Box2D\Box2D.h>
#include <SFML\Graphics.hpp>
#include "CallBack.h"
#include "VecConverter.h"
#include "BoxWorld.h"
#include "ControlMapping.h"
#include "SpriteSheet.h"
#include "ResourceCollection.h"

static const float TIME_UNTIL_FLASHLIGHT_DEATH = 3.0f;

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
		chasing = true;
		//std::cout<<"Contact Begun"<<std::endl;
	}
	
}
void ChaseSensor::endContact(b2Fixture* otherFixture)
{
	if (otherFixture->GetBody()->GetType() == b2_dynamicBody &&  chaseVictim != nullptr && chasing == true)
	{
		chaseVictim = nullptr;
		chasing = false;
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
#pragma region Attack
AttackSensor::AttackSensor(b2Fixture* owner)
	: CallBack(owner)
	, attacking(false)
	, mActive(false)
	, attackVictim(nullptr)

{ }
void AttackSensor::beginContact(b2Fixture* otherFixture)
{
	sf::FloatRect bounds = BoxBounds::boundsOfFixture(otherFixture);
	if (otherFixture->GetBody()->GetType() == b2_dynamicBody && attackVictim == nullptr)
	{
		setVictim(otherFixture, bounds);
		//std::cout<<"Contact Begun"<<std::endl;
	}
	
}
void AttackSensor::endContact(b2Fixture* otherFixture)
{
	if (otherFixture->GetBody()->GetType() == b2_dynamicBody &&  attackVictim != nullptr)
	{
		attacking = false;
		attackVictim = nullptr;
		//std::cout<<"Contact Begun Ended"<<std::endl;
	}
	
}
bool AttackSensor::isAttacking() const
{
	return (attackVictim !=nullptr);
}
bool AttackSensor::isActive() const
{
	return mActive;
}
void AttackSensor::setActive(bool active)
{
	mActive = active;
}
void AttackSensor::setVictim(b2Fixture* fix, const sf::FloatRect& bounds)
{
	attackVictim = fix;
	victimBounds = sf::FloatRect(bounds);
}
#pragma endregion
#pragma region Shade
Shade::Shade(ResourceCollection& resource, BoxWorld* world, sf::Vector2f& size, sf::Vector2f& position)
: EntityLiving(world, size, position)
, mResource(resource)
, collisionFixture(body->GetFixtureList())
, timeInFlashLight(0.0f)
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
	
	auto &attack = mResource.getTexture("Assets/Characters/Shade_attack.png");
	sf::Vector2i attackSize = static_cast<sf::Vector2i>(attack.getSize());
	SpriteSheet attackSheet(frameSize,attackSize);
	std::vector<sf::IntRect> attackFrames = attackSheet.getAllFrames();
	mAttack = new Animation(attackFrames,attack);

	auto &chase = mResource.getTexture("Assets/Characters/Shade_chase.png");
	sf::Vector2i chaseSize = static_cast<sf::Vector2i>(chase.getSize());
	SpriteSheet chaseSheet(frameSize,chaseSize);
	std::vector<sf::IntRect> chaseFrames = chaseSheet.getAllFrames();
	mChase = new Animation(chaseFrames, chase);

	chasingMultiplier = 3.0f;
	speed = 0.5f;
	chaseLength = 1.0f;
	anime.setAnimation(*mWalking);
	updateSpriteOrigin();
	setupSensors(position,size);
	setFacing(LEFT);
	b2Filter filter = (collisionFixture->GetFilterData());
	filter.categoryBits = ENEMY;
	filter.groupIndex = ALL, PLAYER;
	collisionFixture->SetFilterData(filter);
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
	if (timeInFlashLight >= TIME_UNTIL_FLASHLIGHT_DEATH)
	{
		setMode(DYING);
	}

	const b2Vec2& vel = body->GetLinearVelocity();
	if((attackSensorLeft->isAttacking()&& attackSensorLeft->isActive())||(attackSensorRight->isAttacking() && attackSensorRight->isActive())){
		setMode(ATTACK);
	}
	else if((chaseSensorLeft->isChasing()&&chaseSensorLeft->isActive())||(chaseSensorRight->isChasing()&&chaseSensorRight->isActive())){
		setMode(CHASING);
	}
	else{setMode(PATROL);}
	switch(currentMode){
	case PATROL:
		if(getFacing() == LEFT)
		{
				body->SetLinearVelocity(b2Vec2(-speed, vel.y));
		}
		else if(getFacing() == RIGHT)
		{
				body->SetLinearVelocity(b2Vec2(speed, vel.y));
		}
		if(!ledgeSensorLeft->isGrounded())
		{
			setFacing(RIGHT);
		}
		else if(!ledgeSensorRight->isGrounded())
		{
			setFacing(LEFT);
		}
	break;

	case CHASING:
		if(getFacing() == LEFT)
		{
				body->SetLinearVelocity(b2Vec2(-speed*chasingMultiplier, vel.y));
		}
		else if(getFacing() == RIGHT)
		{
				body->SetLinearVelocity(b2Vec2(speed*chasingMultiplier, vel.y));
		}
		if(!ledgeSensorLeft->isGrounded())
		{
			setFacing(RIGHT);
		}
		else if(!ledgeSensorRight->isGrounded())
		{
			setFacing(LEFT);
		}
	break;

	case ATTACK:
		attack();
	break;

	case SPAWN:
	break;

	case DYING:
		mAlive = false;
		break;
	default: 
		setMode(PATROL);
	break;
	}
	if(attackSensorRight->isAttacking()){std::cout<<"Lel"<<std::endl;}
	anime.update(deltaTime);
	updateAnimation();
}
void Shade::setVelocityX(float x)
{
	velocity.x = x;
}
void Shade::setFacing(Facing face)
{
	if(face == getFacing()) return;

	Entity::setFacing(face);
	if(face == LEFT)
	{
		chaseSensorLeft->setActive(true);
		chaseSensorRight->setActive(false);
		attackSensorLeft->setActive(true);
		attackSensorRight->setActive(false);
		std::cout << "LEFT" << std::endl;
	}
	else if(face == RIGHT)
	{ 
		chaseSensorLeft->setActive(false);
		chaseSensorRight->setActive(true);
		attackSensorLeft->setActive(false);
		attackSensorRight->setActive(true);
		std::cout << "RIGHT" << std::endl;
	}
}
void Shade::setupSensors(sf::Vector2f position, sf::Vector2f size)
{
	b2Vec2 bodySize = Convert::sfmlToB2(size);
	b2Vec2 bodyPosRight;
	b2Vec2 bodyPosLeft;
	b2Vec2 groundPosLeft; 
	b2Vec2 groundPosRight;
	b2Vec2 aBodyPosLeft;
	b2Vec2 aBodyPosRight;

	groundPosRight.x = bodySize.x;
	groundPosRight.y = bodySize.y;
	groundPosLeft.y = bodySize.y;
	groundPosLeft.x = bodySize.x-(bodySize.x*2.0f);
	bodyPosLeft.x = bodySize.x-((bodySize.x*3.0f) * chaseLength);
	bodyPosLeft.y = bodySize.y/2;
	bodyPosRight.y = bodySize.y/2;
	bodyPosRight.x = bodySize.x+((bodySize.x*1.2f) * chaseLength);
	aBodyPosLeft.x = bodySize.x-(bodySize.x*1.6f);
	aBodyPosLeft.y = bodySize.y/2;
	aBodyPosRight.x = bodySize.x+(bodySize.x*(-0.4f));
	aBodyPosRight.y = bodySize.y/2;

//Vänster ChaseSensor
	b2PolygonShape shapeLeft;
	shapeLeft.SetAsBox(-1.0f * chaseLength, 0.05f, bodyPosLeft, 0);
	b2FixtureDef defLeft;
	defLeft.isSensor = true;
	defLeft.shape = &shapeLeft;
	b2Fixture* fixLeft = body->CreateFixture(&defLeft);
	chaseSensorLeft = new ChaseSensor(fixLeft);
	fixLeft->SetUserData(chaseSensorLeft);

//Höger ChaseSensor
	b2PolygonShape shapeRight;
	shapeRight.SetAsBox(1.0f * chaseLength, 0.05f, bodyPosRight, 0);
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

//Vänster AttackSensor
	b2PolygonShape aShapeLeft;
	aShapeLeft.SetAsBox(-0.1f, 0.8f, aBodyPosLeft, 0);
	b2FixtureDef aDefLeft;
	aDefLeft.isSensor = true;
	aDefLeft.shape = &aShapeLeft;
	b2Fixture* aFixLeft = body->CreateFixture(&aDefLeft);
	attackSensorLeft = new AttackSensor(aFixLeft);
	aFixLeft->SetUserData(attackSensorLeft);

//Höger AttackSensor
	b2PolygonShape aShapeRight;
	aShapeRight.SetAsBox(0.1f, 0.8f, aBodyPosRight, 0);
	b2FixtureDef aDefRight;
	aDefRight.isSensor = true;
	aDefRight.shape = &aShapeRight;
	b2Fixture* aFixRight = body->CreateFixture(&aDefRight);
	attackSensorRight = new AttackSensor(aFixRight);
	aFixRight->SetUserData(attackSensorRight);

	chaseSensorLeft->setActive(false);
	chaseSensorRight->setActive(false);
	attackSensorLeft->setActive(false);
	attackSensorRight->setActive(false);

//Ändrar kategorier och maskar till de olika sensorerna
	b2Filter filterGroundLeft = groundFixLeft->GetFilterData();
	b2Filter filterGroundRight = groundFixRight->GetFilterData();
	b2Filter filterChaseLeft = fixLeft->GetFilterData();
	b2Filter filterChaseRight = fixRight->GetFilterData();
	b2Filter filterAttackLeft = aFixLeft->GetFilterData();
	b2Filter filterAttackRight = aFixRight->GetFilterData();

	filterGroundLeft.categoryBits = ENEMY_GROUND;
	//filterGroundLeft.maskBits = ALL;

	filterGroundRight.categoryBits = ENEMY_GROUND;
	//filterGroundRight.maskBits = ALL;

	filterChaseLeft.categoryBits = ENEMY_CHASE;
	filterChaseLeft.maskBits = PLAYER;
	filterChaseLeft.groupIndex = PLAYER;

	filterChaseRight.categoryBits = ENEMY_CHASE;
	filterChaseRight.maskBits = PLAYER;
	filterChaseRight.groupIndex = PLAYER;

	filterAttackLeft.categoryBits = ENEMY_ATTACK;
	filterAttackLeft.maskBits = PLAYER;

	filterAttackRight.categoryBits = ENEMY_ATTACK;
	filterAttackRight.maskBits = PLAYER;

	groundFixLeft->SetFilterData(filterGroundLeft);
	groundFixRight->SetFilterData(filterGroundRight);
	fixLeft->SetFilterData(filterChaseLeft);
	fixRight->SetFilterData(filterChaseRight);
	aFixLeft->SetFilterData(filterAttackLeft);
	aFixRight->SetFilterData(filterAttackRight);
}
void Shade::handleAction(Controls::Action action, Controls::KeyState)
{

}
void Shade::updateAnimation()
{
	Animation* currentAnimation = NULL;
	if(currentMode == PATROL)
	{
		currentAnimation = mWalking;
	}
	else if(currentMode == CHASING)
	{
		currentAnimation = mChase;
	}
	else if(currentMode == ATTACK)
	{
		currentAnimation = mAttack;
	}
	else{currentAnimation = mIdle;}
	if (currentAnimation != NULL) anime.play(*currentAnimation);
	//anime.setPosition(Convert::b2ToSfml(body->GetPosition()));

}

void Shade::increaseTimeInFlashLight(float delta)
{
	timeInFlashLight += delta;
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
/* Animationer , dö */
#pragma endregion
