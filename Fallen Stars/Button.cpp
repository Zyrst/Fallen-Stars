#include "Button.h"

Button::Button(sf::Vector2f position, sf::Texture texture, sf::IntRect textureRect, int mID):
	mPosition(position),
	mSprite(texture, textureRect),
	mHighlighted(false)
{
	mSprite.setPosition(position);
}

Button::Button(sf::Vector2f position, sf::Sprite sprite, int mID):
	mPosition(position),
	mSprite(sprite),
	mHighlighted(false)
{
	mSprite.setPosition(position);
}

Button::~Button()
{
}

void Button::render(sf::RenderTarget& renderSurface)
{
	renderSurface.draw(mSprite);
}

void Button::setHighlighted(bool state)
{
	mHighlighted = true;
}

int Button::getID()
{
	return mID;
}

sf::FloatRect Button::getBoundingBox()
{
	return mSprite.getGlobalBounds();
}