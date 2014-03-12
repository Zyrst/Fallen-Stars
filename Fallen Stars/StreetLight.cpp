#include "StreetLight.h"
#include "LightSolver.h"
#include <SFML\Graphics\RectangleShape.hpp>
#include <Box2D\Box2D.h>
#include "VecConverter.h"
#include "BoxWorld.h"
#include "CallBack.h"
#include "Player.h"

namespace
{
	Player* getPlayer(b2Fixture* otherFixture)
	{
		Entity* ent = (Entity*)otherFixture->GetBody()->GetUserData();

		if (ent != nullptr && !otherFixture->IsSensor())
		{
			return dynamic_cast<Player*>(ent);
		}

		return nullptr;
	}

	const float LIT_DURATION_SECONDS = 2.0f;
	const float BLINK_DURATION_SECONDS = 1.5f;
	const int BLINK_INTERVAL_MILLISECONDS = 100;
}

#pragma region StreetLightSensorCallBack
StreetLightSensorCallBack::StreetLightSensorCallBack(b2Fixture* owner)
: CallBack(owner)
, counter(0)
{

}

StreetLightSensorCallBack::~StreetLightSensorCallBack() { }

void StreetLightSensorCallBack::beginContact(b2Fixture* otherFixture)
{
	Player* p = getPlayer(otherFixture);
	if (p != nullptr)
	{
		counter++;
		p->setActiveStreetLight((StreetLight*) owner->GetBody()->GetUserData());
	}
}

void StreetLightSensorCallBack::endContact(b2Fixture* otherFixture)
{
	Player* p = getPlayer(otherFixture);
	if (p != nullptr)
	{
		counter--;

		if (!isActive())
		{
			p->setActiveStreetLight(nullptr);
		}
	}
}

bool StreetLightSensorCallBack::isActive() const
{
	return (counter > 0);
}
#pragma endregion

StreetLight::StreetLight(BoxWorld* world, LightSolver* solver, sf::Vector2f& position, sf::Vector2f& size, const sf::FloatRect& sensor, sf::Texture* mask)
: Entity(world, sf::Vector2f(100, 100), position, LEFT, false)
, lightSource(solver->createLight((int) size.x, (int) size.y))
, sensorBody(world->createEntityBody(sf::Vector2f(sensor.left + sensor.width / 2.0f, sensor.top), sf::Vector2f(sensor.width, sensor.height)))
, clock()
, state(UNLIT)
{ 
	sensorBody->SetUserData(this);
	sf::Vector2f pos = position;
	pos.y += size.y / 2.0f;
	lightSource->setPosition(pos);
	lightSource->setMask(mask);
	lightSource->setColor(sf::Color(255, 0, 0, 150));
	lightSource->setOffset(sf::Vector2f(0.0f, 1.0f));

	int filter = lightSource->getFilterGroup();
	filter |= 1 << 9;
	lightSource->setFilterGroup(filter);

	sensorBody->GetFixtureList()->SetSensor(true);
	sensorCallBack = new StreetLightSensorCallBack(sensorBody->GetFixtureList());
	sensorBody->SetGravityScale(0.0f);
	sensorBody->SetAwake(false);

	body->SetGravityScale(0.0f);
	body->SetAwake(false);

	setState(UNLIT);
}


StreetLight::~StreetLight()
{
	BoxWorld::destroyBody(sensorBody);
}

void StreetLight::render(sf::RenderTarget& target)
{
	//There is nothing to render in this class, everything should be done by the light solver.
	sf::RectangleShape rect(lightSource->getSize());
	sf::Vector2f pos = lightSource->getPosition();
	pos.x -= lightSource->getSize().x / 2.0f;
	pos.y -= lightSource->getSize().y / 2.0f;
	rect.setPosition(pos);

	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineColor(sf::Color::Red);
	rect.setOutlineThickness(2.0f);

	target.draw(rect);
}

void StreetLight::update(sf::Time deltaTime)
{
	sf::Time time = clock.getElapsedTime();
	switch (this->state)
	{
	case LIT:
		if (time.asSeconds() >= LIT_DURATION_SECONDS)
		{
			setState(BLINKING);
		}
		break;

	case BLINKING:
		//You can't explain this shit.
		lightSource->setEnabled((time.asMilliseconds() % (BLINK_INTERVAL_MILLISECONDS * 2)) < BLINK_INTERVAL_MILLISECONDS);

		if (time.asSeconds() >= BLINK_DURATION_SECONDS)
		{
			setState(UNLIT);
		}
		break;
	default:
		break;
	}
}

void StreetLight::handleAction(Controls::Action action, Controls::KeyState)
{

}

StreetLight::StreetLightState StreetLight::getState() const
{
	return this->state;
}

void StreetLight::setState(StreetLight::StreetLightState state)
{
	this->state = state;

	switch (state)
	{
	case UNLIT:
		lightSource->setEnabled(false);
		break;
	case LIT:
		lightSource->setEnabled(true);
		clock.restart();
		break;
	case BLINKING:
		clock.restart();
		break;
	}
}