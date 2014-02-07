#include "Player.h"
#include <iostream>
#include <Box2D\Box2D.h>
#include <SFML\Graphics.hpp>
#include "CallBack.h"
#include "VecConverter.h"
#include "BoxWorld.h"
#include "ControlMapping.h"

GroundCallBack::GroundCallBack(b2Fixture* owner)
	: CallBack(owner)
	, groundTouches(0)
{ }

void GroundCallBack::beginContact(b2Fixture* otherFixture)
{
	if (otherFixture->GetBody()->GetType() == b2_staticBody)
	{
		groundTouches++;
	}
}
void GroundCallBack::endContact(b2Fixture* otherFixture)
{
	if (otherFixture->GetBody()->GetType() == b2_staticBody)
	{
		groundTouches--;
	}
}

bool GroundCallBack::isOnGround()
{
	return (groundTouches > 0);
}

Player::Player(sf::Sprite& sprite, BoxWorld* world, sf::Vector2f& size, sf::Vector2f& position)
: EntityLiving(sprite, world, size, position)
, onGround(false)
, groundCallBack(nullptr)
, leftButton(false)
, rightButton(false)
{
	setupSensors(position, size);
}

Player::~Player()
{
	delete groundCallBack;
}

void Player::setupSensors(sf::Vector2f& pos, sf::Vector2f& size)
{
	b2Vec2 bpos = b2Vec2(0, 0);
	b2Vec2 bsize = Convert::sfmlToB2(size);

	bpos.x += bsize.x / 2;
	bpos.y += bsize.y;

	b2PolygonShape sh;
	sh.SetAsBox(0.1f, 0.1f, bpos, 0);

	b2FixtureDef def;
	def.isSensor = true;
	def.shape = &sh;

	b2Fixture* fix = body->CreateFixture(&def);
	groundCallBack = new GroundCallBack(fix);
	fix->SetUserData(groundCallBack);
}

void Player::update(sf::Time deltaTime)
{
	const b2Vec2& vel = body->GetLinearVelocity();

	if (leftButton)
	{
		body->SetLinearVelocity(b2Vec2(-10, vel.y));
	}
	else if (rightButton)
	{
		body->SetLinearVelocity(b2Vec2(10, vel.y));
	}
	else
	{
		body->SetLinearVelocity(b2Vec2(0, vel.y));
	}
}
void Player::render(sf::RenderTarget& renderTarget)
{
	this->sprite.setPosition(Convert::b2ToSfml(body->GetPosition()));
	renderTarget.draw(this->sprite);
	
	//sprite.setRotation(180);

	sf::FloatRect rect = sprite.getGlobalBounds();

	sf::RectangleShape sh = sf::RectangleShape(sf::Vector2f(rect.width, rect.height));
	sh.setPosition(rect.left, rect.top);
	sh.setFillColor(sf::Color::Transparent);
	sh.setOutlineColor(sf::Color::Red);
	sh.setOutlineThickness(1.0f);

	renderTarget.draw(sh);
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
	if (groundCallBack->isOnGround() && body->GetLinearVelocity().y >= 0)
	{
		const b2Vec2& vel = body->GetLinearVelocity();
		//body->ApplyLinearImpulse(b2Vec2(0, -7), b2Vec2(0, 0), true);
		body->SetLinearVelocity(b2Vec2(vel.x, -10));
	}
}
void Player::setFacing(mFacing face)
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
		break;
	case Controls::Action::RIGHT:
		rightButton = keyDown;
		break;
	case Controls::Action::JUMP:
		if (keyDown)
		{
			jump();
		}	
		break;
	}
}