#pragma once
#include "Entity.h"
#include <SFML/Graphics/Sprite.hpp>
/*
 * EntityLiving best�r utav levande objekt, b�de v�nliga och fientliga
 */
class EntityLiving : public Entity
{
public:
	virtual void render(sf::RenderTarget& renderSurface) override;
	virtual void update(sf::Time deltaTime) override;
	virtual void setVelocity(float x, float y);
	virtual void setVelocityX(float x);
	virtual void setVelocityY(float y);
	virtual bool isAlive();

protected:
	EntityLiving(sf::Sprite& sprite, BoxWorld* world, sf::Vector2f& size, sf::Vector2f& position);
private:
	sf::Vector2f velocity;
};