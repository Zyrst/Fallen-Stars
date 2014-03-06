#pragma once
#include "Entity.h"
#include <SFML/Graphics/Sprite.hpp>
/*
 * EntityLiving contains living objects, both friendly and hostile
 */
class EntityLiving : public Entity
{
public:
	virtual void render(sf::RenderTarget& renderSurface)=0;
	virtual void update(sf::Time deltaTime)=0;
	virtual bool isAlive();

protected:
	EntityLiving(BoxWorld* world, sf::Vector2f& size, sf::Vector2f& position, Facing facing = LEFT);
private:
	sf::Vector2f velocity;
	Facing currentFacing;
};