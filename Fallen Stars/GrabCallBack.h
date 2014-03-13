#pragma once
#include "CallBack.h"
#include <SFML\Graphics\Shape.hpp>

class GrabCallBack : public CallBack
{
public:
	GrabCallBack(b2Fixture* owner);

	virtual void beginContact(b2Fixture* otherFixture) override;
	virtual void endContact(b2Fixture* otherFixture) override;

	bool isColliding() const;
	const sf::FloatRect& getGrabbedFixtureBounds() const;
private:
	void setCandidate(b2Fixture* fix, const sf::FloatRect& bounds);

	b2Fixture* grabCandidate;
	sf::FloatRect candidateBounds;
};