#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Rect.hpp>

// TODO Implement buttons

class Button
{
	public:
		Button(int id, sf::Vector2f position, sf::Texture& texture, sf::Text text = sf::Text(), sf::IntRect textureRect = sf::IntRect());
		~Button();
		void render(sf::RenderTarget& renderSurface);
		void setHighlighted(bool state);
		int getID();
		sf::FloatRect getBoundingBox();

	private:
		sf::Text mText;
		sf::Vector2f mPosition;
		sf::Sprite mSprite;
		bool mHighlighted;
		int mID;
};