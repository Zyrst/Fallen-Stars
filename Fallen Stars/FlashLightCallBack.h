#pragma once
#include "CallBack.h"
#include "ResourceCollection.h"
#include "Player.h"

class FlashLightCallBack : public CallBack
{
public:
	FlashLightCallBack(b2Fixture* owner);
	virtual ~FlashLightCallBack();

	virtual void beginContact(b2Fixture* otherFixture) override;
	virtual void endContact(b2Fixture* otherFixture) override;

private:
//	Shade* closestShade;
	Player* player;
};

