#pragma once

#include "Entity.h"
#include <SFML/Graphics/Sprite.hpp>

class Object: public Entity
{
	public:
		void render(sf::RenderTarget& renderSurface) override;
		void update(sf::Time deltaTime) override;
		sf::Vector2f getPosition() override;
		void setPosition(float x, float y) override;
		bool isAlive();
};