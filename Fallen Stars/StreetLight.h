#pragma once
#include "Entity.h"
#include <SFML\System\Vector2.hpp>
#include "LightSource.h"
#include "CallBack.h"

class LightSolver;

class StreetLightSensorCallBack : public CallBack
{
public:
	StreetLightSensorCallBack(b2Fixture* owner);
	virtual ~StreetLightSensorCallBack();

	virtual void beginContact(b2Fixture* otherFixture) override;
	virtual void endContact(b2Fixture* otherFixture) override;

	bool isActive() const;

private:
	int counter;
};

class StreetLight : public Entity
{
public:
	StreetLight(BoxWorld* world, LightSolver* solver, sf::Vector2f& position, sf::Vector2f& size, const sf::FloatRect& sensor, sf::Texture* mask = nullptr);
	virtual ~StreetLight();

	virtual void render(sf::RenderTarget& renderSurface) override;
	virtual void update(sf::Time deltaTime) override;
	virtual void handleAction(Controls::Action action, Controls::KeyState) override;

private:
	LightSource* lightSource;
	b2Body* sensorBody;
	StreetLightSensorCallBack* sensorCallBack;
};

