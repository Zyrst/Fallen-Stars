#include "FlashLightCallBack.h"
#include <Box2D\Dynamics\b2Fixture.h>
#include "Shade.h"

namespace
{
	Shade* getShade(b2Fixture* fix)
	{
		Entity* e = static_cast<Entity*>(fix->GetBody()->GetUserData());
		return dynamic_cast<Shade*>(e);
	}

	float distancesq(const sf::Vector2f& point1, const sf::Vector2f& point2)
	{
		float x = point1.x - point2.x;
		float y = point1.y - point2.y;

		return (x*x + y*y);
	}
}

FlashLightCallBack::FlashLightCallBack(b2Fixture* owner)
: CallBack(owner)
, shades()
{ 
}


FlashLightCallBack::~FlashLightCallBack()
{ 
	shades.clear();
}

void FlashLightCallBack::beginContact(b2Fixture* otherFixture)
{
	if (!otherFixture->IsSensor())
	{
		Shade* sh = getShade(otherFixture);

		if (sh != nullptr)
		{
			shades.insert(sh);
		}
	}
}

void FlashLightCallBack::endContact(b2Fixture* otherFixture)
{
	if (!otherFixture->IsSensor())
	{
		Entity* e = static_cast<Entity*>(otherFixture->GetBody()->GetUserData());

		if (e != nullptr)
		{
			//It should be safe to static_cast e to a shade (even though e might not be a shade).
			//This is because we never dereference this pointer.
			//dynamic_casting e to shade does not work if the shade is being deleted, since dynamic_cast
			//will then always return null.
			shades.erase(static_cast<Shade*>(e));
		}
	}
}

Shade* FlashLightCallBack::getClosestShade(const sf::Vector2f& position)
{
	Shade* sh = nullptr;
	float distsq = 0.0f;

	for (Shade* s : shades)
	{
		float distsq2 = distancesq(position, s->getPosition());
		if (sh == nullptr || (distsq > distsq2))
		{
			distsq = distsq2;
			sh = s;
		}
	}

	return sh;
}