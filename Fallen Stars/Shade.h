#pragma once

#include <SFML/Audio/Sound.hpp>

#include "Entity.h"
#include "CallBack.h"

class Player;
class ResourceCollection;
class b2Fixture;
class ChaseSensor;
class LedgeSensor;

namespace sf
{
	class Shader;
}

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
	void updateSound();
private:
	Player* player;
	sf::Vector2f velocity;
	void setupSensors(sf::Vector2f position ,sf::Vector2f size);
	ChaseSensor* chaseSensorLeft, *chaseSensorRight, *longChaseSensorLeft, *longChaseSensorRight;
	LedgeSensor* ledgeSensorLeft, *ledgeSensorRight;
	AttackSensor* attackSensorLeft, *attackSensorRight;
	EntityCategory entityCategory;
	ResourceCollection& mResource;
	Animation* mIdle, *mWalking, *mSpawn, *mAttack, *mChase, *mDying;
	Mode currentMode;
	float chasingMultiplier;
	float speed;
	float chaseLength;
	float chaseIncreaseX;
	float chaseIncreaseY;
	b2Fixture* collisionFixture, *aFixLeft, *aFixRight, *groundFixRight, *groundFixLeft;
	b2Fixture* fixRight, *fixLeft, *longFixRight, *longFixLeft;
	sf::Clock hitTimer, deathTimer, turnTimer;
	float timeInFlashLight;
	sf::Shader* shader;
	sf::Sound* mChargeSound, *mAttackSound, *mLaugh;
};

