#include "Shade.h"
#include <iostream>
#include <Box2D\Box2D.h>
#include <SFML\Graphics.hpp>
#include "VecConverter.h"
#include "BoxWorld.h"
#include "ControlMapping.h"
#include "SpriteSheet.h"
#include "ResourceCollection.h"
#include "ChaseSensor.h"
#include "LedgeSensor.h"

static const float TIME_UNTIL_FLASHLIGHT_DEATH = 2.0f;

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
	}
	
}
void AttackSensor::endContact(b2Fixture* otherFixture)
{
	if (otherFixture->GetBody()->GetType() == b2_dynamicBody &&  attackVictim != nullptr)
	{
		attacking = false;
		attackVictim = nullptr;
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
: Entity(world, size, position)
, mResource(resource)
, collisionFixture(body->GetFixtureList())
, timeInFlashLight(0.0f)
, shader(&resource.getShader("Assets/Shader/default.vert", "Assets/Shader/shadeHealth.frag"))
{
	setMode(PATROL);
	sf::Vector2i frameSize(256,256);

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

	auto &dying = mResource.getTexture("Assets/Characters/Shade_dying.png");
	sf::Vector2i dyingSize = static_cast<sf::Vector2i>(dying.getSize());
	SpriteSheet dyingSheet(frameSize,dyingSize);
	std::vector<sf::IntRect> dyingFrames = dyingSheet.getAllFrames();
	mDying = new Animation(dyingFrames, dying);

	//Charge sound
	/*mChargeSound = new sf::Sound;
	mChargeSound->setBuffer(*mResource.getSound("Assets/Sound/"));*/

	//Attack sound
	mAttackSound = new sf::Sound;
	mAttackSound->setBuffer(*mResource.getSound("Assets/Sound/ShadeAttack.wav"));
	mAttackSound->setLoop(false);

	//Laugh
	mLaugh = new sf::Sound;
	mLaugh->setBuffer(*mResource.getSound("Assets/Sound/ShadeLaugh.wav"));
	mLaugh->setLoop(false);

	//Dying Sound
	mDyingSound = new sf::Sound;
	mDyingSound->setBuffer(*mResource.getSound("Assets/Sound/ShadeDying.wav"));
	mDyingSound->setLoop(false);

	chasingMultiplier = 3.0f;
	speed = 0.6f;
	chaseLength = 2.5f;
	chaseIncreaseX = 1.6f;
	chaseIncreaseY = 48.8f;
	anime.setAnimation(*mWalking);
	updateSpriteOrigin();
	setupSensors(position,size);
	setFacing(LEFT);
	b2Filter filter = (collisionFixture->GetFilterData());
	filter.categoryBits = ENEMY;
	filter.groupIndex = ALL, PLAYER;
	collisionFixture->SetFilterData(filter);
	hitTimer.restart();
	turnTimer.restart();
	body->SetFixedRotation(true);
}
Shade::~Shade()
{
	delete mIdle;
	delete mWalking;
	delete mSpawn;
	delete mChargeSound;
	delete mAttackSound;
	delete mLaugh;
}
void Shade::render(sf::RenderTarget& renderTarget, sf::RenderStates states)
{
	anime.setRotation(body->GetAngle() * 180 / 3.14159265f);
	sf::FloatRect rect = anime.getGlobalBounds();
	sf::RectangleShape sh = sf::RectangleShape(sf::Vector2f(rect.width, rect.height));
	sh.setPosition(rect.left, rect.top);
	sh.setFillColor(sf::Color::Transparent);
	sh.setOutlineColor(sf::Color::Blue);
	sh.setOutlineThickness(1.0f);

	float health = (getMode() == DYING) ? 0.0f : timeInFlashLight / TIME_UNTIL_FLASHLIGHT_DEATH;
	shader->setParameter("health", health);
	Entity::render(renderTarget, shader);
}
void Shade::update(sf::Time deltaTime)
{
	const b2Vec2& vel = body->GetLinearVelocity();
	if(currentMode != DYING){
	if(hitTimer.getElapsedTime().asSeconds() <= 3.0f)
	{
		attackSensorLeft->setActive(false);
		attackSensorRight->setActive(false);
	}
	else
	{
		attackSensorLeft->setActive(true);
		attackSensorRight->setActive(true);
	}
	if(turnTimer.getElapsedTime().asSeconds() >= 4.0f&&((longChaseSensorLeft->isChasing() == false ) && (longChaseSensorRight->isChasing() == false)))
	{
		if(getFacing() == LEFT){
		setFacing(RIGHT);
		}
		else if(getFacing() == RIGHT){
		setFacing(LEFT);
		}
	}
	
	if(attackSensorLeft->isAttacking())
	{
		setFacing(LEFT);
		if(attackSensorLeft->isActive())
		{
		attack();
		}
		
	}
	else if(attackSensorRight->isAttacking()){
		setFacing(RIGHT);
		if(attackSensorRight->isActive())
		{
		attack();
		}
		
	}
	else if(((chaseSensorLeft->isChasing()&&chaseSensorLeft->isActive())||(chaseSensorRight->isChasing()&&chaseSensorRight->isActive()))
		|| ((longChaseSensorLeft->isChasing()&&longChaseSensorLeft->isActive())||(longChaseSensorRight->isChasing()&&longChaseSensorRight->isActive()))){
		setMode(CHASING);
	}
	else if(currentMode != DYING) {setMode(PATROL);}

	if (timeInFlashLight >= TIME_UNTIL_FLASHLIGHT_DEATH )
	{
		setMode(DYING);
		mDyingSound->play();
		disableSensors();
	}
	}
	switch(currentMode){
	case PATROL:
		longChaseSensorLeft->setActive(false);
		longChaseSensorRight->setActive(false);
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
		longChaseSensorLeft->setActive(true);
		longChaseSensorRight->setActive(true);
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
		body->SetLinearVelocity(b2Vec2(0, 0));
		anime.setLooped(false);
		if(!anime.isFinished())
		{
			body->SetLinearVelocity(b2Vec2(0,0));
		}
		if(anime.isFinished())
		{
			hitTimer.restart();
			setMode(CHASING);
			attack();
		}
	break;

	case SPAWN:
	break;

	case DYING:
	body->SetLinearVelocity(b2Vec2(0, 0));
	anime.setLooped(false);
	if(!anime.isFinished())
	{
		body->SetLinearVelocity(b2Vec2(0,0));
		body->GetFixtureList()->SetSensor(true);
	}
	if(anime.isFinished())
	{
	hitTimer.restart();
	anime.setLooped(true);
		mAlive = false;
	}	
	break;

	default: 
		setMode(PATROL);
	break;
	}
	anime.update(deltaTime);
	updateAnimation();
	updateSound();
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
		longChaseSensorLeft->setActive(true);
		longChaseSensorRight->setActive(false);
		std::cout << "LEFT" << std::endl;
		turnTimer.restart();
	}
	else if(face == RIGHT)
	{ 
		chaseSensorLeft->setActive(false);
		chaseSensorRight->setActive(true);
		longChaseSensorLeft->setActive(false);
		longChaseSensorRight->setActive(true);
		std::cout << "RIGHT" << std::endl;
		turnTimer.restart();
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
	b2Vec2 lBodyPosLeft;
	b2Vec2 lBodyPosRight;
	
	groundPosRight.x = bodySize.x;
	groundPosRight.y = bodySize.y;
	groundPosLeft.y = bodySize.y;
	groundPosLeft.x = bodySize.x-(bodySize.x*2.0f);
	bodyPosLeft.x = bodySize.x-((bodySize.x*2.8f) * chaseLength);
	bodyPosLeft.y = bodySize.y/2;
	lBodyPosLeft.x = bodySize.x-((bodySize.x*3.1f) * chaseLength);
	lBodyPosLeft.y = bodySize.y/2;
	bodyPosRight.y = bodySize.y/2;
	bodyPosRight.x = bodySize.x+((bodySize.x*0.7f) * chaseLength*chasingMultiplier);
	lBodyPosRight.x = bodySize.x+((bodySize.x*1.0f) * chaseLength*chasingMultiplier);
	lBodyPosRight.y = bodySize.y/2;
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
	fixLeft = body->CreateFixture(&defLeft);
	chaseSensorLeft = new ChaseSensor(fixLeft);
	fixLeft->SetUserData(chaseSensorLeft);

//Höger ChaseSensor
	b2PolygonShape shapeRight;
	shapeRight.SetAsBox(1.0f * chaseLength, 0.05f, bodyPosRight, 0);
	b2FixtureDef defRight;
	defRight.isSensor = true;
	defRight.shape = &shapeRight;
	fixRight = body->CreateFixture(&defRight);
	chaseSensorRight = new ChaseSensor(fixRight);
	fixRight->SetUserData(chaseSensorRight);

//Vänster lång ChaseSensor
	b2PolygonShape lShapeLeft;
	lShapeLeft.SetAsBox(-1.0f * chaseLength* chaseIncreaseX, 0.05f*chaseIncreaseY, lBodyPosLeft, 0);
	b2FixtureDef lDefLeft;
	lDefLeft.isSensor = true;
	lDefLeft.shape = &lShapeLeft;
	longFixLeft = body->CreateFixture(&lDefLeft);
	longChaseSensorLeft = new ChaseSensor(longFixLeft);
	longFixLeft->SetUserData(longChaseSensorLeft);

//Höger lång ChaseSensor
	b2PolygonShape lShapeRight;
	lShapeRight.SetAsBox(1.0f * chaseLength*chaseIncreaseX, 0.05f*chaseIncreaseY, lBodyPosRight, 0);
	b2FixtureDef lDefRight;
	lDefRight.isSensor = true;
	lDefRight.shape = &lShapeRight;
	longFixRight = body->CreateFixture(&lDefRight);
	longChaseSensorRight = new ChaseSensor(longFixRight);
	longFixRight->SetUserData(longChaseSensorRight);

//Vänster Ground sensor
	b2PolygonShape shapeGroundLeft;
	shapeGroundLeft.SetAsBox(-0.05f, -0.15f,groundPosLeft,0);
	b2FixtureDef defGroundLeft;
	defGroundLeft.isSensor = true;
	defGroundLeft.shape = &shapeGroundLeft;
	groundFixLeft = body->CreateFixture(&defGroundLeft);
	ledgeSensorLeft = new LedgeSensor(groundFixLeft);
	groundFixLeft->SetUserData(ledgeSensorLeft);
	
//Höger Ground sensor
	b2PolygonShape shapeGroundRight;
	shapeGroundRight.SetAsBox(0.05f, -0.15f,groundPosRight,0);
	b2FixtureDef defGroundRight;
	defGroundRight.isSensor = true;
	defGroundRight.shape = &shapeGroundRight;
	groundFixRight = body->CreateFixture(&defGroundRight);
	ledgeSensorRight = new LedgeSensor(groundFixRight);
	groundFixRight->SetUserData(ledgeSensorRight);

//Vänster AttackSensor
	b2PolygonShape aShapeLeft;
	aShapeLeft.SetAsBox(-0.1f, 0.8f, aBodyPosLeft, 0);
	b2FixtureDef aDefLeft;
	aDefLeft.isSensor = true;
	aDefLeft.shape = &aShapeLeft;
	aFixLeft = body->CreateFixture(&aDefLeft);
	attackSensorLeft = new AttackSensor(aFixLeft);
	aFixLeft->SetUserData(attackSensorLeft);

//Höger AttackSensor
	b2PolygonShape aShapeRight;
	aShapeRight.SetAsBox(0.1f, 0.8f, aBodyPosRight, 0);
	b2FixtureDef aDefRight;
	aDefRight.isSensor = true;
	aDefRight.shape = &aShapeRight;
	aFixRight = body->CreateFixture(&aDefRight);
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
	b2Filter filterLongChaseLeft = longFixLeft->GetFilterData();
	b2Filter filterLongChaseRight = longFixRight->GetFilterData();

	filterGroundLeft.categoryBits = ENEMY_GROUND;

	filterGroundRight.categoryBits = ENEMY_GROUND;

	filterChaseLeft.categoryBits = ENEMY_CHASE;
	filterChaseLeft.maskBits = PLAYER;

	filterChaseRight.categoryBits = ENEMY_CHASE;
	filterChaseRight.maskBits = PLAYER;

	filterLongChaseLeft.categoryBits = ENEMY_CHASE;
	filterLongChaseLeft.maskBits = PLAYER;

	filterLongChaseRight.categoryBits = ENEMY_CHASE;
	filterLongChaseRight.maskBits = PLAYER;

	filterAttackLeft.categoryBits = ENEMY_ATTACK;
	filterAttackLeft.maskBits = PLAYER;

	filterAttackRight.categoryBits = ENEMY_ATTACK;
	filterAttackRight.maskBits = PLAYER;

	groundFixLeft->SetFilterData(filterGroundLeft);
	groundFixRight->SetFilterData(filterGroundRight);
	fixLeft->SetFilterData(filterChaseLeft);
	fixRight->SetFilterData(filterChaseRight);
	longFixLeft->SetFilterData(filterChaseLeft);
	longFixRight->SetFilterData(filterChaseRight);
	aFixLeft->SetFilterData(filterAttackLeft);
	aFixRight->SetFilterData(filterAttackRight);
}
void Shade::handleAction(Controls::Action action, Controls::KeyState)
{

}
void Shade::updateAnimation()
{
	Animation* currentAnimation = NULL;
	switch(currentMode){
	case PATROL:
	
		currentAnimation = mWalking;
	break;
	case CHASING:
	
		currentAnimation = mChase;
	break;
	case ATTACK:
	
		currentAnimation = mAttack;
	break;
	case DYING:
		currentAnimation = mDying;
	break;
	default: 
		currentAnimation = mIdle;
	break;
	}
	if (currentAnimation != NULL) anime.play(*currentAnimation);
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
	setMode(ATTACK);
}
void Shade::disableSensors()
{
	body->DestroyFixture(aFixLeft);
	body->DestroyFixture(aFixRight);
	body->DestroyFixture(groundFixRight);
	body->DestroyFixture(groundFixLeft);
	body->DestroyFixture(collisionFixture);

	aFixLeft = aFixRight = groundFixLeft = groundFixRight = collisionFixture = nullptr;

	body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
	body->SetGravityScale(0.0f);
}

void Shade::updateSound()
{
	if (currentMode == CHASING)
	{
		if(mLaugh->getLoop() == false)
		{
			mLaugh->play();
			mLaugh->setLoop(true);
		}
	}
	if(currentMode != CHASING)
		{
			mLaugh->stop();
			mLaugh->setLoop(false);
		}

	/*if(currentMode == PATROL)	
	{
		if (Någotljud->getLoop == false)
		{
			//någotljud->play();
			//någotljud->setLoop(true);
		}
	}
	if (currentMode != PATROL)
	{
		//Någotljud->stop();
		//Någotljud->setLoop(false);
	}*/

	if(currentMode == ATTACK)
	{
		if(mAttackSound->getLoop() == false)
		{
			mAttackSound->play();
			mAttackSound->setLoop(true);
		}
	}
	if (currentMode != ATTACK)
	{
		mAttackSound->stop();
		mAttackSound->setLoop(false);
	}

	/*Dying sound plays in the update function due to it playing to late here*/
	
}
#pragma endregion
