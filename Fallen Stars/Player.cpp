#include "Player.h"
#include <Box2D\Box2D.h>
#include <SFML\Graphics.hpp>
#include "CallBack.h"
#include "VecConverter.h"
#include "BoxWorld.h"
#include "ControlMapping.h"
#include "SpriteSheet.h"
#include "LightSolver.h"
#include "ResourceCollection.h"
#include "StatManager.h"
#include "CollisionCounterCallBack.h"
#include "FlashLightCallBack.h"
#include "GrabCallBack.h"
#include "Shade.h"
#include "LightSource.h"
#include "StreetLight.h"
#include "PlatformState.h"

#include <iostream>

static const float SPEED = 3;
static const sf::Vector2i FLASHLIGHT_SIZE(2048, 512);

static sf::Texture* flipTexture(const sf::Texture* source)
{
	sf::RenderTexture fbo;
	fbo.create(source->getSize().x, source->getSize().y);

	sf::Transform trans;
	trans.translate((float)source->getSize().x, 0.0f);
	trans.scale(sf::Vector2f(-1.0f, 1.0f));
	sf::RenderStates states = sf::RenderStates(trans);

	fbo.draw(sf::Sprite(*source), states);

	return new sf::Texture(fbo.getTexture());
}

#pragma region Player
Player::Player(PlatformState& platformState, BoxWorld* world, sf::Vector2f& size, sf::Vector2f& position, ResourceCollection& resource, LightSolver* lightSolver, StatManager& stats)
: Entity(world, size, position)
, mPlatformState(platformState)
, groundCallBack(nullptr)
, leftButton(false)
, rightButton(false)
, downButton(false)
, mResource(resource)
, flashLight(lightSolver->createLight(FLASHLIGHT_SIZE.x, FLASHLIGHT_SIZE.y))
, maskRight(&resource.getTexture("Assets/Shader/mask.png"))
, maskLeft(flipTexture(maskRight))
, activeStreetLight(nullptr)
, collisionFixture(body->GetFixtureList())
, mStats(stats)
, flashLightBody(world->createEntityBody(sf::Vector2f(0.0f, 0.0f), static_cast<sf::Vector2f>(FLASHLIGHT_SIZE), false))
{
	flashLight->setColor(sf::Color(255, 255, 0, 150));
	int group = flashLight->getFilterGroup();
	group |= (1 << 8);
	flashLight->setFilterGroup(group);
	setFilterGroup(getFilterGroup() | (1 << 8));

	setState(PLAYER_STATE::NORMAL);

	/* Walking/run animation */
	auto &walking = mResource.getTexture("Assets/Characters/Stella Left.png");
	sf::Vector2i walkingSize = static_cast<sf::Vector2i>(walking.getSize());
	sf::Vector2i frameSize(256, 256);
	SpriteSheet spritesheet1(frameSize, walkingSize);
	std::vector<sf::IntRect> frames = spritesheet1.getAllFrames();
	mWalking = new Animation(frames,walking);
	
	/* Idle animation */
	auto &idle = mResource.getTexture("Assets/Characters/Stella idle.png");
	sf::Vector2i idleSize = static_cast<sf::Vector2i>(idle.getSize());
	SpriteSheet idleSheet(frameSize,idleSize);
	std::vector<sf::IntRect> idleFrames = idleSheet.getAllFrames();
	mIdle = new Animation(idleFrames,idle);
	
	/* Jump animation */
	auto &jump = mResource.getTexture("Assets/Characters/Stella jump.png");
	sf::Vector2i jumpSize = static_cast<sf::Vector2i>(jump.getSize());
	SpriteSheet jumpSheet(frameSize,jumpSize);
	std::vector<sf::IntRect> jumpFrames = jumpSheet.getAllFrames();
	mJump = new Animation(jumpFrames,jump);

	/* Grab animation */
	auto &grab = mResource.getTexture("Assets/Characters/Stella grab.png");
	sf::Vector2i grabSize = static_cast<sf::Vector2i>(grab.getSize());
	SpriteSheet grabSheet (frameSize, grabSize);
	std::vector<sf::IntRect> grabFrames = grabSheet.getAllFrames();
	mGrab = new Animation(grabFrames,grab);

	/* Fall Animation*/
	auto &fall = mResource.getTexture("Assets/Characters/Stella fall.png");
	sf::Vector2i fallSize = static_cast<sf::Vector2i>(fall.getSize());
	SpriteSheet fallSheet(frameSize, fallSize);
	std::vector<sf::IntRect> fallFrames = jumpSheet.getAllFrames();
	mFall = new Animation(fallFrames,fall);

	/* Hit Animation*/
	auto &hit = mResource.getTexture("Assets/Characters/Stella hit.png");
	sf::Vector2i hitSize = static_cast<sf::Vector2i>(hit.getSize());
	SpriteSheet hitSheet(frameSize, hitSize);
	std::vector<sf::IntRect> hitFrames = hitSheet.getAllFrames();
	mHit = new Animation(hitFrames,hit);

	anime.setAnimation(*mIdle);
	
	updateSpriteOrigin();

	mJumpSound = new sf::Sound;
	mJumpSound->setBuffer(*mResource.getSound("Assets/Sound/Jump.wav"));

	mWalkSound = new sf::Sound;
	mWalkSound->setBuffer(*mResource.getSound("Assets/Sound/Stella_Run_Loop_1.wav"));
	mWalkSound->setVolume(40);

	mHurtSound = new sf::Sound;
	mHurtSound->setBuffer(*mResource.getSound("Assets/Sound/Stella get hurt.wav"));


	setupSensors(position, size);
	body->SetLinearDamping(1.0f);
	/* Set filter for collisions */
	b2Filter filter = collisionFixture->GetFilterData();
	filter.categoryBits = PLAYER;
	//filter.maskBits =ALL, ENEMY_CHASE, ENEMY_ATTACK;
	filter.groupIndex = ALL, ENEMY_CHASE, ENEMY_ATTACK;
	collisionFixture->SetFilterData(filter);
}

Player::~Player()
{
	delete mIdle;
	delete mWalking;
	delete mJump;
	delete mGrab;
	delete mFall;
	delete mJumpSound;
	delete mWalkSound;
	delete mHurtSound;
	BoxWorld::destroyBody(flashLightBody);
}

void Player::setupSensors(sf::Vector2f& pos, sf::Vector2f& size)
{
	b2Vec2 bsize = Convert::sfmlToB2(size);

	//Half width and size.
	const float hw = bsize.x / 2.0f, hh = bsize.y / 2.0f;

	{
		b2Vec2 bpos = b2Vec2(-hw, 0);

		//Setup ground sensor.
		bpos.x += hw;
		bpos.y += bsize.y;

		b2PolygonShape sh;
		sh.SetAsBox(0.2f, 0.01f, bpos, 0);

		b2FixtureDef def;
		def.isSensor = true;
		def.shape = &sh;

		b2Fixture* groundFix = body->CreateFixture(&def);
		groundCallBack = new CollisionCounterCallBack(groundFix);
		b2Filter groundFilter;
		groundFilter = groundFix->GetFilterData();
		groundFilter.categoryBits = PLAYER_SENSOR;
		groundFilter.groupIndex = ALL;
		groundFix->SetFilterData(groundFilter);
		//Left and right side collision sensors (to not get stuck in next to walls anymore)

		bpos = b2Vec2(-hw, 0);
		bpos.y += hh;

		sh.SetAsBox(0.01f, hh*0.99f, bpos, 0);

		b2Fixture* leftFix = body->CreateFixture(&def);
		leftSideCollision = new CollisionCounterCallBack(leftFix);

		b2Filter leftFilter;
		leftFilter = leftFix->GetFilterData();
		leftFilter.categoryBits = PLAYER_SENSOR;
		leftFix->SetFilterData(leftFilter);


		b2Fixture* leftHitFix = body->CreateFixture(&def);
		leftHitCollision = new CollisionCounterCallBack(leftHitFix);

		b2Filter leftHitFilter;
		leftHitFilter = leftHitFix->GetFilterData();
		leftHitFilter.categoryBits = PLAYER_SENSOR;
		leftHitFilter.groupIndex = ENEMY_ATTACK;
		leftHitFix->SetFilterData(leftHitFilter);

		bpos.x += bsize.x;
		sh.SetAsBox(0.01f, hh*0.99f, bpos, 0);

		b2Fixture* rightFix = body->CreateFixture(&def);
		rightSideCollision = new CollisionCounterCallBack(rightFix);

		b2Filter rightFilter;
		rightFilter = rightFix->GetFilterData();
		rightFilter.categoryBits = PLAYER_SENSOR;
		rightFix->SetFilterData(rightFilter);

		b2Fixture* rightHitFix = body->CreateFixture(&def);
		rightHitCollision = new CollisionCounterCallBack(rightHitFix);

		b2Filter rightHitFilter;
		rightHitFilter = leftHitFix->GetFilterData();
		rightHitFilter.categoryBits = PLAYER_SENSOR;
		rightHitFilter.groupIndex = ENEMY_ATTACK;
		rightHitFix->SetFilterData(rightHitFilter);


		//Left and right side grab detectors.
		const float grabYPos = 0.12f;
		const float grabW = 0.05f;
		const float grabH = 0.12f;
		bpos = b2Vec2(-hw - grabW, grabYPos);

		sh.SetAsBox(grabW, grabH, bpos, 0);

		b2Fixture* fix = body->CreateFixture(&def);
		leftGrabCallBack = new GrabCallBack(fix);

		bpos.x += bsize.x + grabW * 2;

		sh.SetAsBox(grabW, grabH, bpos, 0);

		fix = body->CreateFixture(&def);
		rightGrabCallBack = new GrabCallBack(fix);

		//Left and right side anti-grab detectors.
		bpos = b2Vec2(-hw - grabW, grabYPos - grabH - 0.05f);
		sh.SetAsBox(grabW, 0.05f, bpos, 0);

		fix = body->CreateFixture(&def);
		leftAntiGrabCallBack = new CollisionCounterCallBack(fix);

		bpos.x += bsize.x + grabW * 2;
		sh.SetAsBox(grabW, 0.05f, bpos, 0);

		fix = body->CreateFixture(&def);
		rightAntiGrabCallBack = new CollisionCounterCallBack(fix);
	}

	//Flashlight sensors :D
	{
		b2Vec2 size = Convert::sfmlToB2(static_cast<sf::Vector2f>(FLASHLIGHT_SIZE));
		size.x /= 2.0f;
		size.y /= 2.0f;

		size.x *= 0.5;
		float hh = size.y / 2.0f;

		b2PolygonShape shape;
		/*shape.SetAsBox(size.x/4.0f, size.y/2.0f);*/
		
		b2Vec2 points[4];
		points[0] = b2Vec2(0.0f, -hh);
		points[1] = b2Vec2(size.x, -hh);
		points[2] = b2Vec2(size.x, size.y - hh);
		points[3] = b2Vec2(0.0f, size.y - hh);

		shape.Set(points, 4);

		b2FixtureDef def;
		def.isSensor = true;
		def.shape = &shape;
		def.density = 0.0f;

		def.filter.categoryBits = FLASHLIGHT;
		def.filter.groupIndex = ENEMY;
		
		b2Fixture* fix = flashLightBody->CreateFixture(&def);
		fix->SetFilterData(def.filter);
		flashLightBody->SetGravityScale(0.0f);
		flashLightCallBack = new FlashLightCallBack(fix);
	}
}

void Player::update(sf::Time deltaTime)
{
	float maxVel = 3.0f;
	const b2Vec2& vel = body->GetLinearVelocity();
	if(hitTimer.getElapsedTime().asSeconds() <= 3.0f)
	{
		leftHitCollision->setActive(false);
		rightHitCollision->setActive(false);
	}
	else
	{
		leftHitCollision->setActive(true);
		rightHitCollision->setActive(true);
	}

	if(rightHitCollision->isHitColliding() && rightHitCollision->isActive())
	{
		setFacing(RIGHT);
		hitTimer.restart();
		damaged();
	}
	if(leftHitCollision->isHitColliding()&& leftHitCollision->isActive())
	{
		setFacing(LEFT);
		hitTimer.restart();
		damaged();
	}

	if (flashLight->isEnabled())
	{
		Shade* shade = flashLightCallBack->getClosestShade(flashLight->getPosition());
		if (shade)
		{
			Facing dir = (getFacing() == Facing::LEFT) ? RIGHT : LEFT;
			shade->increaseTimeInFlashLight(deltaTime.asSeconds());
			shade->setFacing(dir);
		}
	}

	switch(state)
	{
	case NORMAL:
	
		if ((!rightButton && !leftButton) || (rightButton && leftButton))
		{
			body->SetLinearVelocity(b2Vec2(0,vel.y));
		}
		else if (rightButton)
		{
			if (!rightSideCollision->isColliding() && vel.x < maxVel)
			{
				//body->SetLinearVelocity(b2Vec2(SPEED, vel.y));
				body->ApplyLinearImpulse(b2Vec2(0.8,0.0),b2Vec2(body->GetWorldCenter()),true);
			}
			
			setFacing(Entity::RIGHT);
		}
		else if (leftButton)
		{
			if (!leftSideCollision->isColliding() && vel.x > -maxVel)
			{
				//body->SetLinearVelocity(b2Vec2(-SPEED, vel.y));
				 body->ApplyLinearImpulse(b2Vec2(-0.8,0.0),b2Vec2(body->GetWorldCenter()),true);
			}

			setFacing(Entity::LEFT);
		}
		

		//Check for grabbing if we are not flying upwards.
		if (!groundCallBack->isColliding() && vel.y >= 0)
		{
			if (leftGrabCallBack->isColliding() && !leftAntiGrabCallBack->isColliding() && getFacing() == Facing::LEFT)
			{
				this->setState(PLAYER_STATE::GRABBING);
				const sf::FloatRect& bounds = leftGrabCallBack->getGrabbedFixtureBounds();
				body->SetTransform(Convert::sfmlToB2(sf::Vector2f(bounds.left + bounds.width + (bodyBounds.width / 2.0f), bounds.top)), 0);
			}
			else if (rightGrabCallBack->isColliding() && !rightAntiGrabCallBack->isColliding() && getFacing() == Facing::RIGHT)
			{
				this->setState(PLAYER_STATE::GRABBING);
				const sf::FloatRect& bounds = rightGrabCallBack->getGrabbedFixtureBounds();
				body->SetTransform(Convert::sfmlToB2(sf::Vector2f(bounds.left-(bodyBounds.width/2.0f), bounds.top)), 0);
			}
		}
		
	break;
	case GRABBING:
	
		//Check if we should just drop down
		//Jumping while grabbing is handled in Player::jump() and not here.
		if (downButton || (rightButton && getFacing() == Facing::LEFT) || (leftButton && getFacing() == Facing::RIGHT))
		{
			const float pushDistance = 12.0f;
			//If we are facing right, push the player a bit left
			if (getFacing() == Facing::RIGHT)
			{
				sf::Vector2f pos = Convert::b2ToSfml(body->GetPosition());
				pos.x -= pushDistance;
				body->SetTransform(Convert::sfmlToB2(pos), 0.0f);
				setFacing(Entity::LEFT);
			}
			//Else just push the player a bit right.
			else if (getFacing() == Facing::LEFT)
			{
				sf::Vector2f pos = Convert::b2ToSfml(body->GetPosition());
				pos.x += pushDistance;
				body->SetTransform(Convert::sfmlToB2(pos), 0.0f);
				setFacing(Entity::RIGHT);
			}

			setState(PLAYER_STATE::NORMAL);
		}
	break;
	case DAMAGED:
	anime.setLooped(false);
	if(!anime.isFinished())
	{
		body->SetLinearVelocity(b2Vec2(0,0));
	}
	if(anime.isFinished())
	{
		setState(NORMAL);
		anime.setLooped(true);
	}
	if(mStats.health <=0)
	{
		setState(DYING);
	}
	break;
	case KNOCKEDBACKED:
		
		if (knockedbackClock.getElapsedTime().asSeconds() > 1)
		{
			setState(NORMAL);
		}
		break;
	case DYING:
		mPlatformState.getOverlay(PlatformState::DEATH_SCREEN).setEnabledState(true);
		std::cout<<"Waaaaaaaaaaaah"<<std::endl;
		std::cout<<"---------Player Dead----------"<<std::endl;
		break;
	default:
		break;
	}

	updateAnimation();
	updateSound();
	anime.update(deltaTime);
	updateFlashlightPosition();
}

void Player::updateFlashlightPosition()
{
	const float offsetX = 50.0f;
	float offsetY = 95.0f;
	float bodyOffsetX = 0.0f;

	if (anime.getAnimation() == mWalking)
	{
		offsetY = 106.0f;
	}
	if (anime.getAnimation() == mJump)
	{
		offsetY = 83.0f;
	}

	sf::Vector2f pos = Convert::b2ToSfml(body->GetPosition());
	sf::Texture* mask = nullptr;
	pos.y += offsetY;


	switch (getFacing())
	{
	case LEFT:
		pos.x -= offsetX;
		bodyOffsetX += FLASHLIGHT_SIZE.x / 4.0f;
		mask = maskLeft;
		break;
	case RIGHT:
	default:
		pos.x += offsetX;
		mask = maskRight;
		break;
	}

	flashLight->setPosition(pos);
	flashLight->setMask(mask);

	pos.x -= bodyOffsetX;
	flashLightBody->SetTransform(Convert::sfmlToB2(pos), 0.0f);
}

void Player::render(sf::RenderTarget& renderTarget, sf::RenderStates states)
{
	anime.setRotation(body->GetAngle() * 180 / 3.14159265f);

	if (state == PLAYER_STATE::GRABBING)
	{
		states.transform.translate(0.0f, 22.0f);
	}

	Entity::render(renderTarget, states);
}

void Player::jump()
{
	if (state == PLAYER_STATE::NORMAL)
	{
		if (groundCallBack->isColliding() && body->GetLinearVelocity().y >= 0)
		{
			const b2Vec2& vel = body->GetLinearVelocity();
			//body->ApplyLinearImpulse(b2Vec2(0, -7), b2Vec2(0, 0), true);
			body->SetLinearVelocity(b2Vec2(vel.x, -6));
			mJumpSound->play();
		}

	}
	else if (state == PLAYER_STATE::GRABBING)
	{
		setState(PLAYER_STATE::NORMAL);
		body->SetLinearVelocity(b2Vec2(0, -8));
	}
	
}

void Player::handleAction(Controls::Action action, Controls::KeyState state)
{
	bool keyDown = (state == Controls::KeyState::HELD);

	switch (action)
	{
	case Controls::Action::LEFT:
		leftButton = keyDown;
		break;
	case Controls::Action::RIGHT:
		rightButton = keyDown;
		break;
	case Controls::Action::DOWN:
		downButton = keyDown;
		break;
	case Controls::Action::JUMP:
		if (keyDown)
		{
			jump();
		}	
		break;
	case Controls::Action::INTERACT:
		if (state == Controls::KeyState::RELEASED)
		{
			activateStreetLight();
		}
		break;
	}
	
}

void Player::setState(Player::PLAYER_STATE state)
{
	switch (state)
	{
	case PLAYER_STATE::NORMAL:
		body->SetGravityScale(1.0f);
		collisionFixture->SetSensor(false);
		body->SetAwake(true);
		flashLight->setEnabled(true);
		break;
	case PLAYER_STATE::GRABBING:
		body->SetGravityScale(0.0f);
		body->SetLinearVelocity(b2Vec2(0, 0));
		collisionFixture->SetSensor(true);
		body->SetAwake(false);
		flashLight->setEnabled(false);
		break;
	case PLAYER_STATE::DAMAGED:
		body->SetLinearVelocity(b2Vec2(0, 0));
		mHurtSound->play();
		flashLight->setEnabled(false);
		break;
	case PLAYER_STATE::KNOCKEDBACKED:
		knockedbackClock.restart();
		flashLight->setEnabled(true);
		break;
	}
	this->state = state;
}

void Player::updateAnimation()
{
	currentAnimation = NULL;
	if (leftButton || rightButton)
	{
		currentAnimation = mWalking;
	}

	if(leftButton && rightButton || !leftButton && !rightButton)
	{
		currentAnimation = mIdle;
	}
	/*Jump*/
	if(!groundCallBack->isColliding())
	{
		if (body->GetLinearVelocity().y >= 1)
		{
			currentAnimation = mFall;
		}
		if (body->GetLinearVelocity().y <= 0)
		{
			currentAnimation = mJump;
		}
	}
	if(state == PLAYER_STATE::GRABBING)
	{
		currentAnimation = mGrab;
	}
	if(state == PLAYER_STATE::DAMAGED)
	{
		currentAnimation = mHit;
	}
	if(currentAnimation != NULL) anime.play(*currentAnimation);
	anime.setPosition(Convert::b2ToSfml(body->GetPosition()));
}

void Player::updateSound()
{
	if (leftButton && groundCallBack->isColliding() || rightButton && groundCallBack->isColliding())
	{
		if(mWalkSound->getLoop() == false)
		{
		mWalkSound->play();
		mWalkSound->setLoop(true);
		}
	}
	else
	{
		mWalkSound->stop();
		mWalkSound->setLoop(false);
	}
}

void Player::setActiveStreetLight(StreetLight* light)
{
	this->activeStreetLight = light;
}

b2Body* Player::getBody()
{
	return body;
}

void Player::damaged()
{
	setState(DAMAGED);
	mStats.health -= 1;
	std::cout<<"HP: "<<mStats.health<<std::endl;
}

void Player::activateStreetLight()
{
	if (activeStreetLight && mStats.stardust && (activeStreetLight->getState() == StreetLight::UNLIT))
	{
		mStats.stardust = false;
		activeStreetLight->setState(StreetLight::LIT);
	}
}
#pragma endregion
