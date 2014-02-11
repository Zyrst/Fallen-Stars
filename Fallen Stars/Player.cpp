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

CollisionCounterCallBack::CollisionCounterCallBack(b2Fixture* owner)
	: CallBack(owner)
	, collisions(0)
{
	owner->SetUserData(this);
}

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

GrabCallBack::GrabCallBack(b2Fixture* owner)
	: CallBack(owner)
	, grabCandidate(nullptr)
{
	owner->SetUserData(this);
}

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

Player::Player(/*sf::Texture& texture,*/ BoxWorld* world, sf::Vector2f& size, sf::Vector2f& position,ResourceCollection& resource)
: EntityLiving(world, size, position)
, onGround(false)
, groundCallBack(nullptr)
, leftButton(false)
, rightButton(false)
, downButton(false)
/*, mTexture(texture)*/
, mResource(resource)
{
	auto &run_left = mResource.getTexture("Assets/Map/Stella Left.png");
	auto &run_right = mResource.getTexture("Assets/Map/Shade_walking.png");
	//sf::Vector2i spritesheetSize = static_cast<sf::Vector2i>(texture.getSize());
	sf::Vector2i spritesheetSize1 = static_cast<sf::Vector2i>(run_left.getSize());
	sf::Vector2i spritesheetSize2 = static_cast<sf::Vector2i>(run_right.getSize());
	setState(PLAYER_STATE::NORMAL);

	sf::Vector2i frameSize(256, 256);

	SpriteSheet spritesheet1(frameSize, spritesheetSize1);
	SpriteSheet spritesheet2(frameSize, spritesheetSize2);
	std::vector<sf::IntRect> frames = spritesheet1.getAllFrames();
	std::vector<sf::IntRect> framez = spritesheet2.getAllFrames();
	/*TODO 
	* Make getFrame stuff right, this is incorrect atm*/
	mStellaIdleLeft = new Animation(sf::IntRect(0,0,256,256), mResource.getTexture("Assets/Map/Stella_idle.png"));
	mAnimationWalkLeft = new Animation(frames, mResource.getTexture("Assets/Map/Stella Left.png"));
	mAnimationWalkRight = new Animation (framez,mResource.getTexture("Assets/Map/Shade_walking.png"));

	std::cout << spritesheet1.getFrameCount()<<std::endl;

	anime.setAnimation(*mStellaIdleLeft);
	
	std::cout << mStellaIdleLeft->getSize();

	std::cout << mAnimationWalkRight->getSize()<<std::endl;

	const sf::FloatRect& psize = anime.getLocalBounds();
	anime.setOrigin((psize.width-size.x) / 2.0f, psize.height-size.y);
	setupSensors(position, size);
}

Player::~Player()
{
	delete groundCallBack;
	delete mAnimationWalkRight;
	delete mAnimationWalkLeft;
	delete mStellaIdleLeft;
	//delete mStellaIdleRight;
}

void Player::setupSensors(sf::Vector2f& pos, sf::Vector2f& size)
{

	b2Vec2 bpos = b2Vec2(0, 0);
	b2Vec2 bsize = Convert::sfmlToB2(size);

	//Half width and size.
	const float hw = bsize.x / 2.0f, hh = bsize.y / 2.0f;

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
	bpos = b2Vec2(0, 0);
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
	const float grabH = 0.09f;
	bpos = b2Vec2(0, grabYPos);

	sh.SetAsBox(grabW, grabH, bpos, 0);

	fix = body->CreateFixture(&def);
	leftGrabCallBack = new GrabCallBack(fix);

	bpos.x += bsize.x;

	sh.SetAsBox(grabW, grabH, bpos, 0);

	fix = body->CreateFixture(&def);
	rightGrabCallBack = new GrabCallBack(fix);

	//Left and right side anti-grab detectors.
	bpos = b2Vec2(0, grabYPos - grabH-0.05f);
	sh.SetAsBox(grabW, 0.05f, bpos, 0);

	fix = body->CreateFixture(&def);
	leftAntiGrabCallBack = new CollisionCounterCallBack(fix);

	bpos.x += bsize.x;
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
		}
		else if (rightButton)
		{
			if (!rightSideCollision->isColliding())
			{
				body->SetLinearVelocity(b2Vec2(SPEED, vel.y));
			}
		}
		else
		{
			body->SetLinearVelocity(b2Vec2(0, vel.y));
		}

		//Check for grabbing if we are not flying upwards.
		if (vel.y >= 0)
		{
			if (leftGrabCallBack->isColliding() && !leftAntiGrabCallBack->isColliding())
			{
				this->setState(PLAYER_STATE::GRABBING);
				const sf::FloatRect& bounds = leftGrabCallBack->getGrabbedFixtureBounds();
				body->SetTransform(Convert::sfmlToB2(sf::Vector2f(bounds.left + bounds.width, bounds.top)), 0);
				setFacing(Facing::LEFT);
			}
			else if (rightGrabCallBack->isColliding() && !rightAntiGrabCallBack->isColliding())
			{
				this->setState(PLAYER_STATE::GRABBING);
				const sf::FloatRect& bounds = rightGrabCallBack->getGrabbedFixtureBounds();
				body->SetTransform(Convert::sfmlToB2(sf::Vector2f(bounds.left-bodyBounds.width, bounds.top)), 0);
				setFacing(Facing::RIGHT);
			}
		}
		
	}
	else if (state == PLAYER_STATE::GRABBING)
	{
		//Check if we should just drop down
		//Jumping while grabbing is handled in Player::jump() and not here.
		if (downButton || (rightButton && mFace == Facing::LEFT) || (leftButton && mFace == Facing::RIGHT))
		{
			const float pushDistance = 12.0f;
			//If we are facing right, push the player a bit left
			if (mFace == Facing::RIGHT)
			{
				sf::Vector2f pos = Convert::b2ToSfml(body->GetPosition());
				pos.x -= pushDistance;
				body->SetTransform(Convert::sfmlToB2(pos), 0.0f);
			}
			//Else just push the player a bit right.
			else
			{
				sf::Vector2f pos = Convert::b2ToSfml(body->GetPosition());
				pos.x += pushDistance;
				body->SetTransform(Convert::sfmlToB2(pos), 0.0f);
			}

			setState(PLAYER_STATE::NORMAL);
		}
	}
	

	anime.update(deltaTime);
}

void Player::render(sf::RenderTarget& renderTarget)
{
	Animation* currentAnimation = NULL;
	if (leftButton)
	{
		currentAnimation = mAnimationWalkLeft;
	}
	if (rightButton)
	{
		currentAnimation = mAnimationWalkRight;
	}

	else if(leftButton && rightButton || !leftButton && !rightButton)
	{
		if (mFace == LEFT)
		{
			currentAnimation = mStellaIdleLeft;
		}
		if (mFace == RIGHT)
		{
			//currentAnimation = mStellaIdleRight;
		}
	}
	if(!onGround)
	{
		if (mFace == LEFT)
		{
			/*currentAnimation = Hopp åt vänster */
		}
		else
		{
			/*currentAnimation = Hopp åt höger*/
		}
	}
	
	if(currentAnimation != NULL) anime.play(*currentAnimation);
	anime.setPosition(Convert::b2ToSfml(body->GetPosition()));
	renderTarget.draw(anime);
	
	//sprite.setRotation(180);

	sf::FloatRect rect = anime.getGlobalBounds();

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
	}
}
bool Player::isAlive()
{
	return mAlive;
}
void Player::setVelocityX(float x)
{
	velocity.x = x;
}
void Player::setVelocityY(float y)
{
	velocity.y = y;
}
void Player::setVelocity(float x, float y)
{
	velocity.x = x;
	velocity.y = y;
}
void Player::jump()
{
	if (state == PLAYER_STATE::NORMAL)
	{
		if (groundCallBack->isColliding() && body->GetLinearVelocity().y >= 0)
		{
			const b2Vec2& vel = body->GetLinearVelocity();
			//body->ApplyLinearImpulse(b2Vec2(0, -7), b2Vec2(0, 0), true);
			body->SetLinearVelocity(b2Vec2(vel.x, -5));
		}
	}
	else if (state == PLAYER_STATE::GRABBING)
	{
		setState(PLAYER_STATE::NORMAL);
		body->SetLinearVelocity(b2Vec2(0, -6));
	}
	
}
void Player::setFacing(Facing face)
{
	mFace = face;
}

void Player::handleAction(Controls::Action action, Controls::KeyState state)
{
	bool keyDown = (state == Controls::KeyState::HELD);

	switch (action)
	{
	case Controls::Action::LEFT:
		leftButton = keyDown;
		setFacing(Entity::LEFT);
		break;
	case Controls::Action::RIGHT:
		rightButton = keyDown;
		setFacing(Entity::RIGHT);
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
