#pragma once

#include "Entity.h"
#include <SFML/Graphics/Sprite.hpp>
#include "ResourceCollection.h"
#include "CallBack.h"
#include "EntityLiving.h"
#include "Player.h"
#include "StatManager.h"

class StarCallBack : public CallBack {
public:
	StarCallBack(b2Fixture* owner);

	virtual void beginContact(b2Fixture* otherFixture) override;
	virtual void endContact(b2Fixture* otherFixture) override;

	bool isColliding() const;
	Player* getPlayer();

private:
	 Player* player;
};

class Object: public Entity
{
	public:
		enum TYPE {STAR, STARDUST,WINDOW};
		Object(BoxWorld* world,sf::Vector2f& position, ResourceCollection& resource,TYPE type,StatManager* stats);
		~Object();
		void update(sf::Time deltaTime) override;
		TYPE getType();
		void handleAction(Controls::Action action, Controls::KeyState);
		void render(sf::RenderTarget& target);
		void playSound(TYPE type);
		void setupSensor(sf::Vector2f position);

private:
	TYPE mType;
	ResourceCollection& mResource;
	Animation* mStar, *mStarDust, *mWindow;
	StarCallBack* starCallBack;
	sf::Sound mStarDustSound;
	sf::Sound mStarSound;
	sf::Clock mClock;
	sf::Time mCounter;
	StatManager* mStats;
};