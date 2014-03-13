#pragma once
#include "CallBack.h"
#include <SFML\Graphics\Shape.hpp>

class ChaseSensor : public CallBack
{
public:
	ChaseSensor(b2Fixture* owner);
	virtual void beginContact(b2Fixture* otherFixture) override;
	virtual void endContact(b2Fixture* otherFixture) override;

	bool isChasing() const;
	bool isActive() const;
	void setActive(bool active);
private:
	void setVictim(b2Fixture* fix, const sf::FloatRect& bounds);
	bool chasing;
	bool mActive;
	b2Fixture* chaseVictim;
	sf::FloatRect victimBounds;
};