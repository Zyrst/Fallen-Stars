#pragma once

#include "Entity.h"
#include <SFML/Graphics/Sprite.hpp>
#include "ResourceCollection.h"
#include "CallBack.h"
#include "EntityLiving.h"
#include "Player.h"

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
		enum TYPE {STAR, STARDUST};
		Object(BoxWorld* world,sf::Vector2f& position, ResourceCollection& resource,TYPE type);
		~Object();
		void update(sf::Time deltaTime) override;
		TYPE getType();
		void handleAction(Controls::Action action, Controls::KeyState);
		void render(sf::RenderTarget& target);

private:
	TYPE mType;
	ResourceCollection& mResource;
	Animation* mStar;
	Animation* mStarDust;
	StarCallBack* starCallBack;
};