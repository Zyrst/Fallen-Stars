#pragma once
#include "Entity.h"
#include "CallBack.h"

class Player;
class ResourceCollection;
class b2Fixture;
class ChaseSensor;

class LedgeSensor;

class AttackSensor : public CallBack
{
public:
	AttackSensor(b2Fixture* owner);
	virtual void beginContact(b2Fixture* otherFixture) override;
	virtual void endContact(b2Fixture* otherFixture) override;

	bool isAttacking() const;
	bool isActive() const;
	void setActive(bool active);
private:
	void setVictim(b2Fixture* fix, const sf::FloatRect& bounds);
	bool attacking;
	bool mActive;
	b2Fixture* attackVictim;
	sf::FloatRect victimBounds; 
};

class Shade: public Entity
{
public:
	Shade(ResourceCollection& resource, BoxWorld* world, sf::Vector2f& size, sf::Vector2f& position);
	~Shade();
	void render(sf::RenderTarget& renderSurface, sf::RenderStates states)override;
	void update(sf::Time deltaTime)override;
	void setVelocityX(float x);
	void setFacing(Facing Face);
	void handleAction(Controls::Action action, Controls::KeyState);
	void updateAnimation();
	enum Mode{PATROL, SPAWN, ATTACK, CHASING, IDLE, DYING};
	void attack();
	Mode getMode();
	void setMode(Mode mode);
	void disableSensors();
	void increaseTimeInFlashLight(float delta);
private:
	Player* player;
	sf::Vector2f velocity;
	void setupSensors(sf::Vector2f position ,sf::Vector2f size);
	ChaseSensor* chaseSensorLeft;
	ChaseSensor* chaseSensorRight;
	ChaseSensor* longChaseSensorLeft;
	ChaseSensor* longChaseSensorRight;
	LedgeSensor* ledgeSensorLeft;
	LedgeSensor* ledgeSensorRight;
	AttackSensor* attackSensorLeft;
	AttackSensor* attackSensorRight;
	EntityCategory entityCategory;
	ResourceCollection& mResource;
	Animation* mIdle;
	Animation* mWalking;
	Animation* mSpawn;
	Animation* mAttack;
	Animation* mChase;
	Animation* mDying;
	Mode currentMode;
	float chasingMultiplier;
	float speed;
	float chaseLength;
	float chaseIncreaseX;
	float chaseIncreaseY;
	b2Fixture* collisionFixture;
	sf::Clock hitTimer;
	sf::Clock deathTimer;
	sf::Clock turnTimer;
	float timeInFlashLight;
	b2Fixture* aFixLeft;
	b2Fixture* aFixRight;
	b2Fixture* groundFixRight;
	b2Fixture* groundFixLeft;
	b2Fixture* fixRight;
	b2Fixture* fixLeft;
	b2Fixture* longFixRight;
	b2Fixture* longFixLeft;
};

