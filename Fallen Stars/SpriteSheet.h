#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <vector>

/* Note: This is a helper class for generating frames and doesn't actually contain a texture.
 *
 * Can be used to split a sprite sheet into separate frame rectangles. It simplifies generating 
 * frames by treating the spritesheet as a grid and refering to the frames by their index. It 
 * also makes it possible to generate a whole set of frames at once. */

class SpriteSheet
{
	public:
		SpriteSheet(sf::Vector2i spriteSize, sf::Vector2i spriteSheetSize);

		int getFrameCount();
		sf::IntRect getFrame(int index);
		std::vector<sf::IntRect> getFrames(int startIndex, int frameCount);
		std::vector<sf::IntRect> getAllFrames();

	private:
		/* Size of one animation frame from the spritesheet */
		sf::Vector2i mSpriteSize;
		/* Number of tiles in both axes */
		sf::Vector2i mTiles;
};