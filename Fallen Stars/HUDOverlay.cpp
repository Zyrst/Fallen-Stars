#include "HUDOverlay.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include "BaseResolution.h"

#include <iostream>

HUDOverlay::HUDOverlay(int id, StatManager& stats, ResourceCollection& resources):
	Overlay(id, true),
	mStats(stats),
	mStarDustSprite(resources.getTexture("Assets/GUI/StarDust.png")),
	mStarSprite(resources.getTexture("Assets/GUI/Star.png")),
	mStarText("", resources.getFont("Assets/Menu/24Janvier.otf"), 40u),
	mHPText("", resources.getFont("Assets/Menu/24Janvier.otf"), 40u)
{
	std::cout << "HUD!" << std::endl;
	updateText();
}

void HUDOverlay::updateText()
{
	mStarText.setString(std::to_string(mStats.stars) + "/" + std::to_string(mStats.totalStars));
	mHPText.setString("Health: " + std::to_string(mStats.health));
}

void HUDOverlay::update(const sf::Time& deltaTime)
{
	updateText();
}

void HUDOverlay::render(sf::RenderTarget& renderSurface)
{
	sf::Vector2f centerView = renderSurface.getView().getCenter();
	
	float yOffset = centerView.y - (float)(baseHeight/2);

	mStarText.setPosition(centerView.x + 10, yOffset + 40 - mStarText.getLocalBounds().height/2); 
	mHPText.setPosition(centerView.x + 10, yOffset + 80 - mStarText.getLocalBounds().height/2); 
	mStarSprite.setPosition(centerView.x - mStarSprite.getLocalBounds().width - 10, yOffset + 40 - mStarSprite.getLocalBounds().height/2); 
	mStarDustSprite.setPosition(centerView.x - mStarDustSprite.getLocalBounds().width/2, yOffset + 120);

	renderSurface.draw(mStarSprite);
	renderSurface.draw(mStarText);
	renderSurface.draw(mHPText);
	if(mStats.stardust) renderSurface.draw(mStarDustSprite);
}