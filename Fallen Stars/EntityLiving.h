#pragma once
#include "Entity.h"
#include <SFML/Graphics/Sprite.hpp>
/*
EntityLiving best�r utav levande objekt, b�de v�nliga och fientliga(best�md av enum category.
*/
class EntityLiving : public Entity
{
public:
	EntityLiving(sf::Sprite);
	~EntityLiving();
	bool isAlive();
	void render(sf::RenderWindow window);
	void update(float deltaTime);
private:
	bool mAlive;
}