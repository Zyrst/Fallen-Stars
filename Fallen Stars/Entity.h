#pragma once
#include <SFML/Graphics/RenderWindow.hpp>

/*
Entity är en basklass för alla levande och icke grid-baserade objekt
*/
class Entity
{
public:
	
	virtual void render(sf::RenderWindow window)=0;
	virtual void update(float deltaTime)=0;
	bool isAlive();
	void collidesWith(Entity* other);

protected:
	Entity();

	bool mAlive;
}