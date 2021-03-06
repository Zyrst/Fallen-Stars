#pragma once

#include "AnimatedSprite.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>

/* A simple object representing static animated background images. These images are placed when
 * the level is loaded and don't move. They only passively loop their animation. */

class AnimatedBackgroundImage
{
public:
	explicit AnimatedBackgroundImage(sf::Vector2f position, sf::Vector2i spriteSize, const sf::Texture& texture, sf::Time frameTime = sf::seconds(0.1f));
	~AnimatedBackgroundImage();
	void update(sf::Time deltaTime);
	void render(sf::RenderTarget& renderSurface);

private: 
	Animation* mAnimation;
	AnimatedSprite mAnimatedSprite;
};

