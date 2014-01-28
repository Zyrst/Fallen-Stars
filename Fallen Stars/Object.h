#pragma once
#include "Entity.h"
#include <SFML/Graphics/Sprite.hpp>
class Object: public Entity
{
public:
	bool isAlive();
	void render(sf::RenderWindow window);
	void update(float deltaTime);
	sf::Vector2f getPosition();
	void setPosition(float x, float y);
private:
	bool mAlive;
	sf::Vector2f position;
	sf::Sprite sprite;
	sf::IntRect boundingBox;

};