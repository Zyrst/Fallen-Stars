#pragma once
#include "EntityLiving.h"
#include "Player.h"

class ChaseSensor : public CallBack
{
public:
	ChaseSensor(b2Fixture* owner);
	virtual void beginContact(b2Fixture* otherFixture) override;
	virtual void endContact(b2Fixture* otherFixture) override;

	bool isChasing() const;
	bool isActive() const;
	void setActive(bool active);
private:
	void setVictim(b2Fixture* fix, const sf::FloatRect& bounds);
	bool chasing;
	bool mActive;
	b2Fixture* chaseVictim;
	sf::FloatRect victimBounds; 
};

class LedgeSensor : public CallBack
{
public:
	LedgeSensor(b2Fixture* owner);
	virtual void beginContact(b2Fixture* otherFixture) override;
	virtual void endContact(b2Fixture* otherFixture) override;
	bool isGrounded() const;
private:
	void setFinder(b2Fixture* fix, const sf::FloatRect& bounds);
	b2Fixture* groundFinder;
	sf::FloatRect finderBounds;
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
	enum Mode{PATROL, SPAWN, ATTACK};
	void attack();
	Mode getMode();
	void setMode(Mode mode);
private:
	Player* player;
	sf::Vector2f velocity;
	void setupSensors(sf::Vector2f position ,sf::Vector2f size);
	ChaseSensor* chaseSensorLeft;
	ChaseSensor* chaseSensorRight;
	LedgeSensor* ledgeSensorLeft;
	LedgeSensor* ledgeSensorRight;
	EntityCategory entityCategory;
	ResourceCollection& mResource;
	Animation* mIdle;
	Animation* mWalking;
	Animation* mSpawn;
	Mode currentMode;
};

