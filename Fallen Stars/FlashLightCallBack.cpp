#include "FlashLightCallBack.h"
#include <Box2D\Dynamics\b2Fixture.h>
#include <iostream>
#include "Shade.h"
namespace
{
	Shade* getShade(b2Fixture* fix)
	{
		Entity* e = static_cast<Entity*>(fix->GetBody()->GetUserData());
		return dynamic_cast<Shade*>(e);
	}
}

FlashLightCallBack::FlashLightCallBack(b2Fixture* owner)
: CallBack(owner)
//, closestShade(nullptr)
{ }


FlashLightCallBack::~FlashLightCallBack()
{ }

void FlashLightCallBack::beginContact(b2Fixture* otherFixture)
{
	Shade* sh = getShade(otherFixture);

	if (sh != nullptr)
	{
		std::cout << "Colliding with shade :D" << "\n";
	}
}

void FlashLightCallBack::endContact(b2Fixture* otherFixture)
{
	Shade* sh = getShade(otherFixture);
}