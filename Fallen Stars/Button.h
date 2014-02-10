#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Rect.hpp>

// TODO Implement buttons

class Button
{
	public:
		Button(sf::Vector2f position, sf::Texture texture, sf::IntRect textureRect, int mID);
		Button(sf::Vector2f position, sf::Sprite sprite, int mID);
		~Button();
		void render(sf::RenderTarget& renderSurface);
		void setHighlighted(bool state);
		int getID();
		sf::FloatRect getBoundingBox();

	private:
		sf::Vector2f mPosition;
		sf::Sprite mSprite;
		bool mHighlighted;
		int mID;
};