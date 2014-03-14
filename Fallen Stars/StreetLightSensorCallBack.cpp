#include "StreetLightSensorCallBack.h"
#include <Box2D\Dynamics\b2Fixture.h>
#include "Player.h"

static Player* getPlayer(b2Fixture* otherFixture)
{
	Entity* ent = (Entity*) otherFixture->GetBody()->GetUserData();

	if (ent != nullptr && !otherFixture->IsSensor())
	{
		return dynamic_cast<Player*>(ent);
	}

	return nullptr;
}

StreetLightSensorCallBack::StreetLightSensorCallBack(b2Fixture* owner)
: CallBack(owner)
, counter(0)
{

}

StreetLightSensorCallBack::~StreetLightSensorCallBack() { }

void StreetLightSensorCallBack::beginContact(b2Fixture* otherFixture)
{
	Player* p = getPlayer(otherFixture);
	if (p != nullptr)
	{
		counter++;
		p->setActiveStreetLight((StreetLight*) owner->GetBody()->GetUserData());
	}
}

void StreetLightSensorCallBack::endContact(b2Fixture* otherFixture)
{
	Player* p = getPlayer(otherFixture);
	if (p != nullptr)
	{
		counter--;

		if (!isActive())
		{
			p->setActiveStreetLight(nullptr);
		}
	}
}

bool StreetLightSensorCallBack::isActive() const
{
	return (counter > 0);
}