#pragma once

#include "Entity.h"
#include <SFML/Graphics/Sprite.hpp>

class Object: public Entity
{
	public:
		void render(sf::RenderTarget renderSurface) override;
		void update(float deltaTime) override;
		sf::Vector2f getPosition();
		void setPosition(float x, float y) override;
	private:
		sf::Vector2f position; // duplicerad ?
		sf::Sprite sprite; // duplicerad ?
		sf::IntRect boundingBox; // duplicerad ?

};