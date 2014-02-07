#pragma once
#include "EntityLiving.h"
#include "CallBack.h"
#include "Animation.h"

class GroundCallBack : public CallBack
{
public:
	GroundCallBack(b2Fixture* owner);

	virtual void beginContact(b2Fixture* otherFixture) override;
	virtual void endContact(b2Fixture* otherFixture) override;

	bool isOnGround();

private:
	int groundTouches;
};

class Player : public EntityLiving
{
public:
	Player(sf::Texture& texture, BoxWorld* world, sf::Vector2f& size, sf::Vector2f& position);
	~Player();
	void render(sf::RenderTarget& renderSurface) override;
	void update(sf::Time deltaTime) override;
	void setVelocityX(float x);
	void setVelocityY(float y);
	void setVelocity(float x, float y);
	void jump();
	bool isAlive();
	void setFacing(mFacing face);
	virtual void handleAction(Controls::Action action, Controls::KeyState);
private:
	void setupSensors(sf::Vector2f& pos, sf::Vector2f& size);

	sf::Vector2f velocity;
	mFacing mFace;
	GroundCallBack* groundCallBack;
	bool onGround, leftButton, rightButton;
	Animation* mAnimationWalkRight;
	Animation* mAnimationWalkLeft;
};