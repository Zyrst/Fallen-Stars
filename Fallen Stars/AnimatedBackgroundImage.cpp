#include "AnimatedBackgroundImage.h"

#include "SpriteSheet.h"

AnimatedBackgroundImage::AnimatedBackgroundImage(sf::Vector2f position, sf::Vector2i spriteSize, const sf::Texture& texture, sf::Time frameTime):
	mAnimatedSprite(),
	mAnimation(NULL)
{
	SpriteSheet spritesheet(spriteSize, static_cast<sf::Vector2i>(texture.getSize()));
	mAnimation = new Animation(spritesheet.getAllFrames(), texture, frameTime);
	mAnimatedSprite.setPosition(position);
	mAnimatedSprite.setAnimation(*mAnimation);
}

AnimatedBackgroundImage::~AnimatedBackgroundImage()
{
	delete mAnimation;
}

void AnimatedBackgroundImage::update(sf::Time deltaTime)
{
	mAnimatedSprite.update(deltaTime);
}

void AnimatedBackgroundImage::render(sf::RenderTarget& renderSurface)
{
	renderSurface.draw(mAnimatedSprite);
}