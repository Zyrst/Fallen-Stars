#pragma once
#include "CallBack.h"

class CollisionCounterCallBack : public CallBack
{
public:
	CollisionCounterCallBack(b2Fixture* owner);

	virtual void beginContact(b2Fixture* otherFixture) override;
	virtual void endContact(b2Fixture* otherFixture) override;

	bool isColliding() const;
	bool isHitColliding() const;

private:
	int collisions;
	int hitCollisions;
};