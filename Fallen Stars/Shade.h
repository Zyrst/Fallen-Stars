#pragma once
#include "EntityLiving.h"
#include "Player.h"
class ChaseSensor : public CallBack
{
public:
	ChaseSensor(b2Fixture* owner);
	virtual void beginContact(b2Fixture* otherFixture) override;
	virtual void endContact(b2Fixture* otherFixture) override;

	bool isChasing();
	bool isActive();
	void setActive(bool active);
private:
	int chasing;
	bool mActive;
};

class LedgeSensor : public CallBack
{
public:
	LedgeSensor(b2Fixture* owner);
	virtual void beginContact(b2Fixture* otherFixture) override;
	virtual void endContact(b2Fixture* otherFixture) override;
	bool isGrounded() const;
private:
	int grounded;
	bool mActive;
};

class Shade: public EntityLiving
{
public:
	Shade(ResourceCollection& resource, BoxWorld* world, sf::Vector2f& size, sf::Vector2f& position);
	~Shade();
	void render(sf::RenderTarget& renderSurface)override;
	void update(sf::Time deltaTime)override;
	void setVelocityX(float x);
	void setFacing(Facing Face);
	void handleAction(Controls::Action action, Controls::KeyState);
	void updateAnimation();
private:
	Player* player;
	sf::Vector2f velocity;
	void setupSensors(sf::Vector2f position ,sf::Vector2f size);
	Facing mFace;
	ChaseSensor* chaseSensorLeft;
	ChaseSensor* chaseSensorRight;
	LedgeSensor* ledgeSensorLeft;
	LedgeSensor* ledgeSensorRight;
	EntityCategory entityCategory;
	ResourceCollection& mResource;
	Animation* mIdle;
	Animation* mWalking;
	Animation* mSpawn;
};

