#include "StreetLight.h"
#include "LightSolver.h"
#include <SFML\Graphics\RectangleShape.hpp>
#include <Box2D\Box2D.h>
#include "VecConverter.h"
#include "BoxWorld.h"
#include "CallBack.h"
#include "Player.h"

#include <iostream>

namespace
{
	bool isPlayer(b2Fixture* otherFixture)
	{
		Entity* ent = (Entity*)otherFixture->GetBody()->GetUserData();

		if (ent != nullptr && !otherFixture->IsSensor())
		{
			Player* player = dynamic_cast<Player*>(ent);

			if (player != nullptr)
			{
				return true;
			}
		}

		return false;
	}
}

StreetLightSensorCallBack::StreetLightSensorCallBack(b2Fixture* owner)
: CallBack(owner)
, counter(0)
{

}

StreetLightSensorCallBack::~StreetLightSensorCallBack() {}

void StreetLightSensorCallBack::beginContact(b2Fixture* otherFixture)
{
	if (isPlayer(otherFixture))
	{
		counter++;
	}
}

void StreetLightSensorCallBack::endContact(b2Fixture* otherFixture)
{
	if (isPlayer(otherFixture))
	{
		counter--;
	}
}

bool StreetLightSensorCallBack::isActive() const
{
	return (counter > 0);
}

StreetLight::StreetLight(BoxWorld* world, LightSolver* solver, sf::Vector2f& position, sf::Vector2f& size, const sf::FloatRect& sensor, sf::Texture* mask)
: Entity(world, sf::Vector2f(100, 100), position, LEFT, false)
, lightSource(solver->createLight(size.x, size.y))
, sensorBody(world->createEntityBody(sf::Vector2f(sensor.left + sensor.width/2.0f, sensor.top), sf::Vector2f(sensor.width, sensor.height)))
{ 
	sf::Vector2f pos = position;
	/*pos.x += size.x / 2.0f;*/
	pos.y += size.y / 2.0f;
	lightSource->setPosition(pos);
	lightSource->setMask(mask);
	lightSource->setColor(sf::Color(255, 0, 0, 100));
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
}


StreetLight::~StreetLight()
{ }

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
	lightSource->setEnabled(sensorCallBack->isActive());
}

void StreetLight::handleAction(Controls::Action action, Controls::KeyState)
{

}