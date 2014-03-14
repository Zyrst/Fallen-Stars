#pragma once
#include "CallBack.h"

class StreetLightSensorCallBack : public CallBack
{
public:
	StreetLightSensorCallBack(b2Fixture* owner);
	virtual ~StreetLightSensorCallBack();

	virtual void beginContact(b2Fixture* otherFixture) override;
	virtual void endContact(b2Fixture* otherFixture) override;

	bool isActive() const;

private:
	int counter;
};