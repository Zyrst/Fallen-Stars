#include "ChaseSensor.h"
#include <Box2D\Dynamics\b2Fixture.h>
#include "BoxWorld.h"

ChaseSensor::ChaseSensor(b2Fixture* owner)
: CallBack(owner)
, chasing(false)
, mActive(false)
, chaseVictim(nullptr)
{ }
void ChaseSensor::beginContact(b2Fixture* otherFixture)
{
	sf::FloatRect bounds = BoxBounds::boundsOfFixture(otherFixture);
	if (otherFixture->GetBody()->GetType() == b2_dynamicBody && chaseVictim == nullptr)
	{
		setVictim(otherFixture, bounds);
		chasing = true;
	}

}
void ChaseSensor::endContact(b2Fixture* otherFixture)
{
	if (otherFixture->GetBody()->GetType() == b2_dynamicBody &&  chaseVictim != nullptr && chasing == true)
	{
		chaseVictim = nullptr;
		chasing = false;
	}

}
bool ChaseSensor::isChasing() const
{
	return (chaseVictim != nullptr);
}
bool ChaseSensor::isActive() const
{
	return mActive;
}
void ChaseSensor::setActive(bool active)
{
	mActive = active;
}
void ChaseSensor::setVictim(b2Fixture* fix, const sf::FloatRect& bounds)
{
	chaseVictim = fix;
	victimBounds = sf::FloatRect(bounds);
}