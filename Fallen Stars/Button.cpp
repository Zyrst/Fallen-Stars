#include "Button.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include "ResourceCollection.h"

Button::Button(int id, sf::Vector2f position, sf::Texture& texture, ResourceCollection& resources, sf::Text text, bool enabled, float textOffset, sf::IntRect textureRect):
	mText(text),
	mPosition(position),
	mSprite(texture),
	mHighlightShader(resources.getShader("Assets/Shader/Highlight.frag", sf::Shader::Type::Fragment)),
	mHighlighted(false),
	mEnabled(enabled),
	mID(id)
{
	if(textureRect != sf::IntRect()) mSprite.setTextureRect(textureRect);
	
	sf::FloatRect spriteBounds = mSprite.getLocalBounds();
	mSprite.setPosition(position - sf::Vector2f(spriteBounds.width, spriteBounds.height) / 2.0f);
	
	sf::FloatRect textBounds = mText.getLocalBounds();
	mText.setPosition(position - sf::Vector2f(textBounds.width + spriteBounds.width * textOffset, textBounds.height) / 2.0f);
}

Button::~Button()
{
}

void Button::render(sf::RenderTarget& renderSurface)
{
	float highlightStrength = mHighlighted ? 1.2f : 1.0f;
	mHighlightShader.setParameter("highlightStrength", mEnabled ? highlightStrength : 0.6f);
	mHighlightShader.setParameter("texture", sf::Shader::CurrentTexture);
	renderSurface.draw(mSprite, &mHighlightShader);
	mText.setColor(sf::Color(20, 20, 100));
	renderSurface.draw(mText);
}

void Button::setHighlighted(bool state)
{
	mHighlighted = state;
}

bool Button::isEnabled()
{
	return mEnabled;
}

int Button::getID()
{
	return mID;
}

sf::FloatRect Button::getBoundingBox()
{
	return mSprite.getGlobalBounds();
}