#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
/*
Entity är en basklass för alla levande och icke grid-baserade objekt
*/
class Entity
{
public:
	//TODO Update skall få funktioner
	//TODO collisionWith måste ta emot boundingBoxes
	virtual void render(sf::RenderWindow window)=0;
	virtual void update(float deltaTime)=0;
	virtual sf::Vector2f getPosition()=0;
	virtual void setPosition(float x,float y)=0;
	bool isAlive();
	void collidesWith(Entity* other);

protected:
	Entity(sf::Sprite sprite, sf::IntRect boundingBox, sf::Vector2f position);
	sf::Sprite sprite;
	sf::IntRect boundingBox;
	sf::Vector2f position;
	bool mAlive;
};