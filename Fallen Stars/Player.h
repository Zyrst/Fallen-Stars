#pragma once
#include "Entity.h"
#include <SFML/Audio/Sound.hpp>
#include <SFML\System\Clock.hpp>

class LightSolver;
class Animation;
class ResourceCollection;
struct StatManager;
class CollisionCounterCallBack;
class FlashLightCallBack;
class GrabCallBack;
class LightSource;
class StreetLight;
class b2Fixture;
class PlatformState;
class CallBack;

class Player : public Entity
{
public:
	enum PLAYER_STATE { NORMAL, GRABBING, DAMAGED, KNOCKEDBACKED, DYING };
	Player(PlatformState& platformState, BoxWorld* world, sf::Vector2f& size, sf::Vector2f& position, ResourceCollection& resource, LightSolver* lightSolver, StatManager& stats);
	virtual ~Player();
	void render(sf::RenderTarget& renderSurface, sf::RenderStates states) override;
	void update(sf::Time deltaTime) override;
	void jump();
	virtual void handleAction(Controls::Action action, Controls::KeyState);
	void updateAnimation();
	void updateSound();
	void setState(PLAYER_STATE state);
	void setActiveStreetLight(StreetLight* light);
	void damaged();
	void activateStreetLight();
	b2Body* getBody();
private:
	void setupSensors(sf::Vector2f& pos, sf::Vector2f& size);
	void updateFlashlightPosition();
	int knockForce;
	bool toggle;
	sf::Texture *maskRight, *maskLeft;
	sf::Sound* mJumpSound, *mWalkSound, *mHurtSound;
	sf::Clock hitTimer, knockedbackClock;
	sf::Vector2f velocity;
	PLAYER_STATE state;
	CollisionCounterCallBack *groundCallBack, *rightSideCollision, *leftSideCollision, *leftAntiGrabCallBack, *rightAntiGrabCallBack, *rightHitCollision, *leftHitCollision, *shadeCollision;
	GrabCallBack *leftGrabCallBack, *rightGrabCallBack;
	FlashLightCallBack* flashLightCallBack;
	Animation* mWalking, *mIdle, *mJump, *mGrab, *mFall, *currentAnimation, *mHit;
	ResourceCollection& mResource;
	LightSource* flashLight;
	StatManager& mStats;
	StreetLight* activeStreetLight;
	b2Fixture* collisionFixture;
	b2Body* flashLightBody;
	PlatformState& mPlatformState;
	bool leftButton, rightButton, downButton;
};