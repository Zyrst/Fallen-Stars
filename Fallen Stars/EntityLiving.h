#pragma once
#include "Entity.h"
#include <SFML/Graphics/Sprite.hpp>
/*
EntityLiving består utav levande objekt, både vänliga och fientliga(bestämd av enum category.
*/
class EntityLiving : public Entity
{
public:
	bool isAlive();
	void render(sf::RenderWindow window);
	void update(float deltaTime);
	sf::Vector2f getPosition();
	void setPosition(float x, float y);
	void setVelocity(float x, float y);
private:
	bool mAlive;
	sf::Vector2f position;
	sf::Vector2f velocity;
	sf::Sprite sprite;
	sf::IntRect boundingBox;
};