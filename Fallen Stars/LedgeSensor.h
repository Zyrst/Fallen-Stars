#pragma once
#include "CallBack.h"
#include <SFML\Graphics\Shape.hpp>

class LedgeSensor : public CallBack
{
public:
	LedgeSensor(b2Fixture* owner);
	virtual void beginContact(b2Fixture* otherFixture) override;
	virtual void endContact(b2Fixture* otherFixture) override;
	bool isGrounded() const;
private:
	void setFinder(b2Fixture* fix, const sf::FloatRect& bounds);
	b2Fixture* groundFinder;
	sf::FloatRect finderBounds;
};