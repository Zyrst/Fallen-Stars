#include "LedgeSensor.h"
#include <Box2D\Dynamics\b2Fixture.h>
#include "BoxWorld.h"

LedgeSensor::LedgeSensor(b2Fixture* owner)
: CallBack(owner)
, groundFinder(nullptr)
{ }
void LedgeSensor::beginContact(b2Fixture* otherFixture)
{
	sf::FloatRect bounds = BoxBounds::boundsOfFixture(otherFixture);
	if (otherFixture->GetBody()->GetType() == b2_staticBody && groundFinder == nullptr)
	{
		setFinder(otherFixture, bounds);
		//std::cout<<"G";
	}
}
void LedgeSensor::endContact(b2Fixture* otherFixture)
{
	if (otherFixture->GetBody()->GetType() == b2_staticBody && groundFinder != nullptr)
	{
		groundFinder = nullptr;
		//std::cout<<"420 ground not found lel"<<std::endl;
	}
}
bool LedgeSensor::isGrounded() const
{
	return (groundFinder != nullptr);
}
void LedgeSensor::setFinder(b2Fixture* fix, const sf::FloatRect& bounds)
{
	groundFinder = fix;
	finderBounds = sf::FloatRect(bounds);
}