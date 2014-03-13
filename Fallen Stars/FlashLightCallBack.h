#pragma once
#include "CallBack.h"
#include <SFML\System\Vector2.hpp>
#include <set>

class Entity;
class Shade;

class FlashLightCallBack : public CallBack
{
public:
	FlashLightCallBack(b2Fixture* owner);
	virtual ~FlashLightCallBack();

	virtual void beginContact(b2Fixture* otherFixture) override;
	virtual void endContact(b2Fixture* otherFixture) override;

	Shade* getClosestShade(const sf::Vector2f& position);

private:
	std::set<Shade*> shades;
};

