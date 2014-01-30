#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

// TODO Implement buttons

class Button
{
	public:
		Button(sf::Vector2f position);
		~Button();
		void render(sf::RenderTarget& renderSurface);

	private:
		sf::Vector2f mPosition;
};