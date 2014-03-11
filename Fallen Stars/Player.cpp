#include "Player.h"
#include <Box2D\Box2D.h>
#include <SFML\Graphics.hpp>
#include "CallBack.h"
#include "VecConverter.h"
#include "BoxWorld.h"
#include "ControlMapping.h"
#include "SpriteSheet.h"
#include "ResourceCollection.h"
#include "LightSolver.h"
#include <SFML/Audio/SoundBuffer.hpp>

#include <iostream>

static const float SPEED = 3;
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

#pragma region CollisionCounterCallBack
CollisionCounterCallBack::CollisionCounterCallBack(b2Fixture* owner)
	: CallBack(owner)
	, collisions(0)
{ }

void CollisionCounterCallBack::beginContact(b2Fixture* otherFixture)
{
	if (otherFixture->GetBody()->GetType() == b2_staticBody)
	{
		collisions++;
	}
}
void CollisionCounterCallBack::endContact(b2Fixture* otherFixture)
{
	if (otherFixture->GetBody()->GetType() == b2_staticBody)
	{
		collisions--;
	}
}

bool CollisionCounterCallBack::isColliding() const
{
	return (collisions > 0);
}
#pragma endregion
#pragma region GrabCallBack
GrabCallBack::GrabCallBack(b2Fixture* owner)
	: CallBack(owner)
	, grabCandidate(nullptr)
{ }

void GrabCallBack::beginContact(b2Fixture* otherFixture)
{
	sf::FloatRect bounds = BoxBounds::boundsOfFixture(otherFixture);
	if (otherFixture->GetBody()->GetType() == b2_staticBody && (grabCandidate == nullptr || bounds.top < candidateBounds.top))
	{
		setCandidate(otherFixture, bounds);
	}
}

void GrabCallBack::endContact(b2Fixture* otherFixture)
{
	if (otherFixture->GetBody()->GetType() == b2_staticBody && grabCandidate != nullptr && otherFixture == grabCandidate)
	{
		grabCandidate = nullptr;
	}
}

bool GrabCallBack::isColliding() const
{
	return (grabCandidate != nullptr);
}

const sf::FloatRect& GrabCallBack::getGrabbedFixtureBounds() const
{
	return candidateBounds;
}

void GrabCallBack::setCandidate(b2Fixture* fix, const sf::FloatRect& bounds)
{
	grabCandidate = fix;
	candidateBounds = sf::FloatRect(bounds);
}
#pragma endregion
#pragma region Player
Player::Player(BoxWorld* world, sf::Vector2f& size, sf::Vector2f& position, ResourceCollection& resource, LightSolver* lightSolver)
: EntityLiving(world, size, position)
, groundCallBack(nullptr)
, leftButton(false)
, rightButton(false)
, downButton(false)
, mResource(resource)
, flashLight(lightSolver->createLight(2048, 512))
, maskRight(&resource.getTexture("Assets/Shader/mask.png"))
, maskLeft(flipTexture(maskRight))
, activeStreetLight(nullptr)
, collisionFixture(body->GetFixtureList())
{
	flashLight->setColor(sf::Color(255, 255, 0, 10));
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
	auto &grab = mResource.getTexture("Assets/Characters/Stella_grabLeft.png");
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

	anime.setAnimation(*mIdle);
	
	updateSpriteOrigin();

	mJumpSound = new sf::Sound;
	mJumpSound->setBuffer(*mResource.getSound("Assets/Sound/Jump.wav"));

	mWalkSound = new sf::Sound;
	mWalkSound->setBuffer(*mResource.getSound("Assets/Sound/Stella_Run_Loop_1.wav"));
	mWalkSound->setVolume(75);


	setupSensors(position, size);
	body->SetLinearDamping(1.0f);
	/* Set filter for collisions */
	b2Filter filter = (body->GetFixtureList())->GetFilterData();
	filter.categoryBits = PLAYER;
	filter.maskBits = ENEMY_CHASE, ENEMY_ATTACK;
	//filter.groupIndex = ALL, ENEMY_CHASE;
	body->GetFixtureList()->SetFilterData(filter);
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
}

void Player::setupSensors(sf::Vector2f& pos, sf::Vector2f& size)
{
	b2Vec2 bsize = Convert::sfmlToB2(size);

	//Half width and size.
	const float hw = bsize.x / 2.0f, hh = bsize.y / 2.0f;

	b2Vec2 bpos = b2Vec2(-hw, 0);

	//Setup ground sensor.
	bpos.x += hw;
	bpos.y += bsize.y;

	b2PolygonShape sh;
	sh.SetAsBox(0.2f, 0.01f, bpos, 0);

	b2FixtureDef def;
	def.isSensor = true;
	def.shape = &sh;

	b2Fixture* fix = body->CreateFixture(&def);
	groundCallBack = new CollisionCounterCallBack(fix);
	b2Filter groundFilter;
	groundFilter = fix->GetFilterData();
	groundFilter.categoryBits = PLAYER_SENSOR;

	fix->SetFilterData(groundFilter);
	//Left and right side collision sensors (to not get stuck in next to walls anymore)
	bpos = b2Vec2(-hw, 0);
	bpos.y += hh;

	sh.SetAsBox(0.01f, hh*0.99f, bpos, 0);

	fix = body->CreateFixture(&def);
	leftSideCollision = new CollisionCounterCallBack(fix);

	bpos.x += bsize.x;
	sh.SetAsBox(0.01f, hh*0.99f, bpos, 0);

	fix = body->CreateFixture(&def);
	rightSideCollision = new CollisionCounterCallBack(fix);

	//Left and right side grab detectors.
	const float grabYPos = 0.12f;
	const float grabW = 0.05f;
	const float grabH = 0.12f;
	bpos = b2Vec2(-hw-grabW, grabYPos);

	sh.SetAsBox(grabW, grabH, bpos, 0);

	fix = body->CreateFixture(&def);
	leftGrabCallBack = new GrabCallBack(fix);

	bpos.x += bsize.x+grabW*2;

	sh.SetAsBox(grabW, grabH, bpos, 0);

	fix = body->CreateFixture(&def);
	rightGrabCallBack = new GrabCallBack(fix);

	//Left and right side anti-grab detectors.
	bpos = b2Vec2(-hw-grabW, grabYPos - grabH-0.05f);
	sh.SetAsBox(grabW, 0.05f, bpos, 0);

	fix = body->CreateFixture(&def);
	leftAntiGrabCallBack = new CollisionCounterCallBack(fix);

	bpos.x += bsize.x + grabW*2;
	sh.SetAsBox(grabW, 0.05f, bpos, 0);

	fix = body->CreateFixture(&def);
	rightAntiGrabCallBack = new CollisionCounterCallBack(fix);
}

void Player::update(sf::Time deltaTime)
{
	const b2Vec2& vel = body->GetLinearVelocity();
	switch(state)
	{
	case NORMAL:
	
		if (leftButton)
		{
			if (!leftSideCollision->isColliding())
			{
				body->SetLinearVelocity(b2Vec2(-SPEED, vel.y));
			}

			setFacing(Entity::LEFT);
		}
		else if (rightButton)
		{
			if (!rightSideCollision->isColliding())
			{
				body->SetLinearVelocity(b2Vec2(SPEED, vel.y));
			}
			
			setFacing(Entity::RIGHT);
		}
		else
		{
			body->SetLinearVelocity(b2Vec2(0, vel.y));
		}

		if (downButton && activeStreetLight != nullptr && activeStreetLight->getState() == StreetLight::UNLIT)
		{
			activeStreetLight->setState(StreetLight::LIT);
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
			else
			{
				sf::Vector2f pos = Convert::b2ToSfml(body->GetPosition());
				pos.x += pushDistance;
				body->SetTransform(Convert::sfmlToB2(pos), 0.0f);
				setFacing(Entity::RIGHT);
			}

			setState(PLAYER_STATE::NORMAL);
		}
	break;
	default:
		break;
	}
	updateAnimation();
	updateSound();
	anime.update(deltaTime);
	//anime.setPosition(Convert::b2ToSfml(body->GetPosition()));

	updateFlashlightPosition();
}

void Player::updateFlashlightPosition()
{
	const float offsetX = 50.0f;
	float offsetY = 95.0f;

	if (anime.getAnimation() == mWalking)
	{
		offsetY = 106.0f;
	}

	sf::Vector2f pos = Convert::b2ToSfml(body->GetPosition());
	sf::Texture* mask = nullptr;
	pos.y += offsetY;

	if (getFacing() == Facing::LEFT)
	{
		pos.x -= offsetX;
		mask = maskLeft;
	}
	else
	{
		pos.x += offsetX;
		mask = maskRight;
	}

	flashLight->setPosition(pos);
	flashLight->setMask(mask);
}

void Player::render(sf::RenderTarget& renderTarget)
{
	anime.setRotation(body->GetAngle() * 180 / 3.14159265f);
	Entity::render(renderTarget);
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
		break;
	case PLAYER_STATE::GRABBING:
		body->SetGravityScale(0.0f);
		body->SetLinearVelocity(b2Vec2(0, 0));
		collisionFixture->SetSensor(true);
		body->SetAwake(false);
		break;
	}
	this->state = state;
}

void Player::updateAnimation()
{
	Animation* currentAnimation = NULL;
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
#pragma endregion
