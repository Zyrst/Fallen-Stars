#include "Button.h"

Button::Button(int id, sf::Vector2f position, sf::Texture& texture, sf::Text text, sf::IntRect textureRect):
	mText(text),
	mPosition(position),
	mSprite(texture),
	mHighlighted(false),
	mID(id)
{
	if(textureRect != sf::IntRect()) mSprite.setTextureRect(textureRect);
	
	sf::FloatRect spriteBounds = mSprite.getLocalBounds();
	mSprite.setPosition(position - sf::Vector2f(spriteBounds.width, spriteBounds.height) / 2.0f);
	
	sf::FloatRect textBounds = mText.getLocalBounds();
	mText.setPosition(position - sf::Vector2f(textBounds.width, textBounds.height) / 2.0f);
}

Button::~Button()
{
}

void Button::render(sf::RenderTarget& renderSurface)
{
	renderSurface.draw(mSprite);
	renderSurface.draw(mText);
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