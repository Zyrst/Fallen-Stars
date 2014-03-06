#include "StreetLight.h"
#include "LightSolver.h"
#include <SFML\Graphics\RectangleShape.hpp>

StreetLight::StreetLight(BoxWorld* world, LightSolver* solver, sf::Vector2f& position, sf::Vector2f& size, sf::Texture* mask)
: Entity(world, sf::Vector2f(0, 0), position, LEFT, false)
, lightSource(solver->createLight(size.x, size.y, 1.0f))
{ 
	sf::Vector2f pos = position;
	/*pos.x += size.x / 2.0f;
	pos.y += size.y / 2.0f;*/
	lightSource->setPosition(pos);
	lightSource->setMask(mask);
	lightSource->setColor(sf::Color(255, 0, 0, 255));
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
	rect.setOutlineThickness(1.0f);

	target.draw(rect);
}

void StreetLight::update(sf::Time deltaTime)
{

}

void StreetLight::handleAction(Controls::Action action, Controls::KeyState)
{

}