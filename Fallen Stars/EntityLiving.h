#pragma once
#include "Entity.h"
#include <SFML/Graphics/Sprite.hpp>
/*
 * EntityLiving best�r utav levande objekt, b�de v�nliga och fientliga
 */
class EntityLiving : public Entity
{
	public:
		void render(sf::RenderTarget* renderSurface) override;
		void update(float deltaTime) override;
		sf::Vector2f getPosition() override;
		void setPosition(float x, float y);
		void setVelocity(float x, float y);
		bool isAlive();
	private:
		sf::Vector2f velocity;

};