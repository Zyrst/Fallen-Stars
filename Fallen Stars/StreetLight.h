#pragma once
#include "Entity.h"
#include <SFML\System\Vector2.hpp>
#include <SFML\System\Clock.hpp>

class LightSolver;
class StreetLightSensorCallBack;
class LightSource;
class CallBack;

class StreetLight : public Entity
{
public:
	enum StreetLightState {UNLIT, LIT, BLINKING};

	StreetLight(BoxWorld* world, LightSolver* solver, sf::Vector2f& position, sf::Vector2f& size, const sf::FloatRect& sensor, sf::Texture* mask = nullptr);
	virtual ~StreetLight();

	virtual void render(sf::RenderTarget& renderSurface, sf::RenderStates states) override;
	virtual void update(sf::Time deltaTime) override;
	virtual void handleAction(Controls::Action action, Controls::KeyState) override;

	StreetLightState getState() const;
	void setState(StreetLightState st);

private:
	LightSource* lightSource;
	b2Body* sensorBody;
	StreetLightSensorCallBack* sensorCallBack;
	sf::Clock clock;
	StreetLightState state;
};

