#include "Player.h"
#include <iostream>
#include <Box2D\Box2D.h>
#include <SFML\Graphics.hpp>
#include "CallBack.h"
#include "VecConverter.h"
#include "BoxWorld.h"
#include "ControlMapping.h"
#include "SpriteSheet.h"
#include "ResourceCollection.h"
#include <iostream>

static const float SPEED = 3;

/*
* TODO:Fix falling through while grabbing (maybe three grab boxes?)
*/

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
/*----*/
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
/*----*//*
OnesideCallBack::OnesideCallBack(b2Fixture* owner)
	: CallBack(owner)
	, collisions(0)
{

}
void OnesideCallBack::beginContact(b2Fixture* otherFixture)
{
		if (otherFixture->GetBody()->GetType() == b2_staticBody&& otherFixture->)
	{
		collisions++;
	}
}
void OnesideCallBack::endContact(b2Fixture* otherFixture)
{
		if (otherFixture->GetBody()->GetType() == b2_staticBody)
	{
		collisions--;
	}
}

bool OnesideCallBack::isColliding() const
{
	return(collisions > 0);
}
*/

Player::Player(BoxWorld* world, sf::Vector2f& size, sf::Vector2f& position,ResourceCollection& resource)
: EntityLiving(world, size, position)
, groundCallBack(nullptr)
, leftButton(false)
, rightButton(false)
, downButton(false)
, mResource(resource)
{

	setState(PLAYER_STATE::NORMAL);

	/* Walking/run animation */
	auto &walking = mResource.getTexture("Assets/Map/Stella Left.png");
	sf::Vector2i walkingSize = static_cast<sf::Vector2i>(walking.getSize());
	sf::Vector2i frameSize(256, 256);
	SpriteSheet spritesheet1(frameSize, walkingSize);
	std::vector<sf::IntRect> frames = spritesheet1.getAllFrames();
	mWalking = new Animation(frames,walking);
	
	std::cout << spritesheet1.getFrameCount()<<std::endl;
	std::cout << mWalking->getSize()<<std::endl;
	
	/* Idle animation */
	auto &idle = mResource.getTexture("Assets/Map/Stella_idle.png");
	sf::Vector2i idleSize = static_cast<sf::Vector2i>(idle.getSize());
	SpriteSheet idleSheet(frameSize,idleSize);
	std::vector<sf::IntRect> idleFrames = idleSheet.getAllFrames();
	mIdle = new Animation(idleFrames,idle);
	
	/* Jump animation */
	auto &jump = mResource.getTexture("Assets/Map/Stella_jumpLeft.png");
	sf::Vector2i jumpSize = static_cast<sf::Vector2i>(jump.getSize());
	SpriteSheet jumpSheet(frameSize,jumpSize);
	std::vector<sf::IntRect> jumpFrames = jumpSheet.getAllFrames();
	mJump = new Animation(jumpFrames,jump);

	/* Grab animation */
	auto &grab = mResource.getTexture("Assets/Map/Stella_grabLeft.png");
	sf::Vector2i grabSize = static_cast<sf::Vector2i>(grab.getSize());
	SpriteSheet grabSheet (frameSize, grabSize);
	std::vector<sf::IntRect> grabFrames = grabSheet.getAllFrames();
	mGrab = new Animation(grabFrames,grab);


	std::cout << mIdle->getSize()<<std::endl;

	anime.setAnimation(*mIdle);
	
	updateSpriteOrigin();

	mJumpSound = mResource.getSound("Assets/Sound/Jump.wav");
	mWalkSound = mResource.getSound("Assets/Sound/Walking 2.wav");

	setupSensors(position, size);
	body->SetLinearDamping(1.0f);
	/* Set filter for collisions */
	b2Filter filter = (body->GetFixtureList())->GetFilterData();
	filter.categoryBits = PLAYER;
	//filter.groupIndex = ALL, ENEMY_CHASE;
	body->GetFixtureList()->SetFilterData(filter);
}

Player::~Player()
{
	delete groundCallBack;
	delete mIdle;
	delete mWalking;
	delete mJump;
	delete mGrab;
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
	const float grabYPos = 0.05f;
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

	if (state == PLAYER_STATE::NORMAL)
	{
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
		
	}
	else if (state == PLAYER_STATE::GRABBING)
	{
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
	}
	
	updateAnimation();
	updateSound();
	anime.update(deltaTime);
}

void Player::render(sf::RenderTarget& renderTarget)
{
	anime.setRotation(body->GetAngle() * 180 / 3.14159265f);
	Entity::render(renderTarget);

	/*sf::FloatRect rect = anime.getGlobalBounds();

	sf::RectangleShape sh = sf::RectangleShape(sf::Vector2f(rect.width, rect.height));
	sh.setPosition(rect.left, rect.top);
	sh.setFillColor(sf::Color::Transparent);
	sh.setOutlineColor(sf::Color::Red);
	sh.setOutlineThickness(1.0f);

	renderTarget.draw(sh);

	//Ugly debug code incoming to check for grabbed fixtures.
	if (leftGrabCallBack->isColliding())
	{
		const sf::FloatRect& bounds = leftGrabCallBack->getGrabbedFixtureBounds();
		sh.setSize(sf::Vector2f(bounds.width, bounds.height));
		sh.setPosition(bounds.left, bounds.top);
		sh.setFillColor(sf::Color(0, 0, 200, 200));

		renderTarget.draw(sh);

		sh.setFillColor(sf::Color(255, 0, 0, 255));
		sh.setSize(sf::Vector2f(10, 10));
		sh.setPosition(bounds.left + bounds.width - sh.getSize().x, bounds.top);

		renderTarget.draw(sh);
	}

	if (rightGrabCallBack->isColliding())
	{
		const sf::FloatRect& bounds = rightGrabCallBack->getGrabbedFixtureBounds();
		sh.setSize(sf::Vector2f(bounds.width, bounds.height));
		sh.setPosition(bounds.left, bounds.top);
		sh.setFillColor(sf::Color(0, 0, 200, 200));

		renderTarget.draw(sh);

		sh.setFillColor(sf::Color(255, 0, 0, 255));
		sh.setSize(sf::Vector2f(10, 10));

		renderTarget.draw(sh);
	}*/
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
			mJumpSound.play();
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
		body->SetAwake(true);
		break;
	case PLAYER_STATE::GRABBING:
		body->SetGravityScale(0.0f);
		body->SetLinearVelocity(b2Vec2(0, 0));
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
		if(groundCallBack->isColliding())
		{
		/*This is to make so the Stella doesn't move while both buttons are down*/
		/*But we can't jump while standing still so not working as intended*/
		//body->SetLinearVelocity(b2Vec2(0, velocity.y));
		}
	}
	if(!groundCallBack->isColliding())
	{
		currentAnimation = mJump;
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
		if(mWalkSound.getLoop() == false)
		{
		mWalkSound.play();
		mWalkSound.setLoop(true);
		}
		std::cout << "Walking soundssss" << std::endl;
	}
	else
	{
		mWalkSound.stop();
		mWalkSound.setLoop(false);
	}
}