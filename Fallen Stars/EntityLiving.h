#pragma once
#include "Entity.h"
#include <SFML/Graphics/Sprite.hpp>
/*
 * EntityLiving best�r utav levande objekt, b�de v�nliga och fientliga
 */
class EntityLiving : public Entity
{
public:
	virtual void render(sf::RenderTarget& renderSurface)=0;
	virtual void update(sf::Time deltaTime)=0;
	virtual bool isAlive();

protected:
	EntityLiving(BoxWorld* world, sf::Vector2f& size, sf::Vector2f& position);
private:
	sf::Vector2f velocity;
};