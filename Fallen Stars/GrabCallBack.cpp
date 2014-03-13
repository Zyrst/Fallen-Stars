#include "GrabCallBack.h"
#include <Box2D\Dynamics\b2Fixture.h>
#include "BoxWorld.h"

GrabCallBack::GrabCallBack(b2Fixture* owner)
: CallBack(owner)
, grabCandidate(nullptr)
{ }

void GrabCallBack::beginContact(b2Fixture* otherFixture)
{
	sf::FloatRect bounds = BoxBounds::boundsOfFixture(otherFixture);
	if (otherFixture->GetBody()->GetType() == b2_staticBody && (grabCandidate == nullptr || bounds.top < candidateBounds.top))
	{
		setCandidate(otherFixture, bounds);
	}
}

void GrabCallBack::endContact(b2Fixture* otherFixture)
{
	if (otherFixture->GetBody()->GetType() == b2_staticBody && grabCandidate != nullptr && otherFixture == grabCandidate)
	{
		grabCandidate = nullptr;
	}
}

bool GrabCallBack::isColliding() const
{
	return (grabCandidate != nullptr);
}

const sf::FloatRect& GrabCallBack::getGrabbedFixtureBounds() const
{
	return candidateBounds;
}

void GrabCallBack::setCandidate(b2Fixture* fix, const sf::FloatRect& bounds)
{
	grabCandidate = fix;
	candidateBounds = sf::FloatRect(bounds);
}