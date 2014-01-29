#pragma once
#include "Entity.h"
#include <SFML/Graphics/Sprite.hpp>
/*
 * EntityLiving består utav levande objekt, både vänliga och fientliga(bestämd av enum category.
 */
class EntityLiving : public Entity
{
	public:
		void render(sf::RenderTarget renderSurface) override;
		void update(float deltaTime) override;
		sf::Vector2f getPosition() override;
		void setPosition(float x, float y);
		void setVelocity(float x, float y);
	private:
		sf::Vector2f position; // duplicerad ?
		sf::Vector2f velocity;
		sf::Sprite sprite; // duplicerad ?
		sf::IntRect boundingBox; // duplicerad ?
};