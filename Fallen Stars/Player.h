#pragma once
#include "EntityLiving.h"
#include "CallBack.h"
#include "Animation.h"
#include "ResourceCollection.h"
#include "LightSource.h"
#include "StreetLight.h"

class LightSolver;

class CollisionCounterCallBack : public CallBack
{
public:
	CollisionCounterCallBack(b2Fixture* owner);

	virtual void beginContact(b2Fixture* otherFixture) override;
	virtual void endContact(b2Fixture* otherFixture) override;

	bool isColliding() const;

private:
	int collisions;
};
class GrabCallBack : public CallBack
{
public:
	GrabCallBack(b2Fixture* owner);

	virtual void beginContact(b2Fixture* otherFixture) override;
	virtual void endContact(b2Fixture* otherFixture) override;

	bool isColliding() const;
	const sf::FloatRect& getGrabbedFixtureBounds() const;
private:
	void setCandidate(b2Fixture* fix, const sf::FloatRect& bounds);

	b2Fixture* grabCandidate;
	sf::FloatRect candidateBounds;
};
class Player : public EntityLiving
{
public:
	enum PLAYER_STATE { NORMAL, GRABBING, DAMAGED };
	Player(BoxWorld* world, sf::Vector2f& size, sf::Vector2f& position, ResourceCollection& resource, LightSolver* lightSolver);
	~Player();
	void render(sf::RenderTarget& renderSurface) override;
	void update(sf::Time deltaTime) override;
	void jump();
	virtual void handleAction(Controls::Action action, Controls::KeyState);
	void updateAnimation();
	void updateSound();
	void setState(PLAYER_STATE state);
	void setActiveStreetLight(StreetLight* light);
	b2Body* getBody();
private:
	void setupSensors(sf::Vector2f& pos, sf::Vector2f& size);
	void updateFlashlightPosition();

	sf::Vector2f velocity;
	PLAYER_STATE state;
	CollisionCounterCallBack *groundCallBack, *rightSideCollision, *leftSideCollision, *leftAntiGrabCallBack, *rightAntiGrabCallBack;
	GrabCallBack *leftGrabCallBack, *rightGrabCallBack;
	bool leftButton, rightButton, downButton;
	Animation* mWalking;
	Animation* mIdle;
	Animation* mJump;
	Animation* mGrab;
	Animation* mFall;
	sf::Sound mJumpSound;
	sf::Sound mWalkSound;
	ResourceCollection& mResource;

	LightSource* flashLight;
	sf::Texture *maskRight, *maskLeft;

	StreetLight* activeStreetLight;

	b2Fixture* collisionFixture;
};