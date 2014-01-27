#pragma once
#include "Entity.h"
#include <SFML/Graphics/Sprite.hpp>
class Object: public Entity
{
public:

	Object(sf::Sprite);
	~Object();
	bool isAlive();
	void render(sf::RenderWindow window);
	void update(float deltaTime);
private:
	bool mAlive;

}