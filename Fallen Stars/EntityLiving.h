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
		virtual sf::Vector2f getPosition() override;
		virtual void setPosition(float x, float y);
		virtual void setPositionX(float x);
		virtual void setPositionY(float y);
		virtual void setVelocity(float x, float y);
		virtual bool isAlive();

	private:
		sf::Vector2f velocity;
};