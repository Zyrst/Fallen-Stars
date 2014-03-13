#include "CollisionCounterCallBack.h"
#include <Box2D\Dynamics\b2Fixture.h>
#include "Shade.h"

namespace
{
	bool isShade(b2Fixture* b)
	{
		Entity* e = static_cast<Entity*>(b->GetBody()->GetUserData());
		return (dynamic_cast<Shade*>(e) != nullptr);
	}
}

CollisionCounterCallBack::CollisionCounterCallBack(b2Fixture* owner)
: CallBack(owner)
, collisions(0)
, hitCollisions(0)
, active(true)
{ }

void CollisionCounterCallBack::beginContact(b2Fixture* otherFixture)
{
	if (otherFixture->GetBody()->GetType() == b2_staticBody)
	{
		collisions++;
	}
	if (otherFixture->GetBody()->GetType() == b2_dynamicBody && isShade(otherFixture))
	{
		hitCollisions++;
	}
}
void CollisionCounterCallBack::endContact(b2Fixture* otherFixture)
{
	if (otherFixture->GetBody()->GetType() == b2_staticBody)
	{
		collisions--;
	}
	if (otherFixture->GetBody()->GetType() == b2_dynamicBody && isShade(otherFixture))
	{
		hitCollisions--;
	}
}

bool CollisionCounterCallBack::isColliding() const
{
	return (collisions > 0);
}
bool CollisionCounterCallBack::isHitColliding() const
{
	return (hitCollisions > 0);
}
bool CollisionCounterCallBack::isActive() const
{
	return active;
}
void CollisionCounterCallBack::setActive(bool mActive)
{
	active = mActive;
}