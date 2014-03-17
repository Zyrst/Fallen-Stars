#include "ConstellationOverlay.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include "ResourceCollection.h"
#include "Game.h"
#include "MainMenuState.h"

#include <iostream>

const sf::Time fadeInTime = sf::seconds(2.5f);
const sf::Time visibleTime = sf::seconds(2.0f);
const sf::Time fadeOutTime = sf::seconds(1.5f);

ConstellationOverlay::ConstellationOverlay(int id, ResourceCollection& resources, std::string levelname):
	Overlay(id, false),
	constellation(resources.getTexture("Assets/Puzzle/" + levelname + "/Constellation.png")),
	elapsedTime(sf::seconds(0.0f)),
	currentMode(FADE_IN)
{
}

void ConstellationOverlay::update(const sf::Time& deltaTime)
{
	elapsedTime += deltaTime;

	if(currentMode == FADE_IN)
	{
		if(elapsedTime > fadeInTime)
		{
			swapMode(VISIBLE);
		}
	}
	else if(currentMode == VISIBLE)
	{
		if(elapsedTime > visibleTime)
		{
			swapMode(FADE_OUT);
		}
	}
	else if(currentMode == FADE_OUT)
	{
		if(elapsedTime > fadeOutTime)
		{
			Game::instance()->loadNewState(new MainMenuState());
		}
	}
}

void ConstellationOverlay::render(sf::RenderTarget& renderSurface)
{
	if(currentMode == FADE_IN)
	{
		int brightness = (int) (255 * elapsedTime.asSeconds() / fadeInTime.asSeconds());
		constellation.setColor(sf::Color(255,255,255,brightness));
		renderSurface.draw(constellation);
	}
	if(currentMode == VISIBLE)
	{
		constellation.setColor(sf::Color(255,255,255,255));
		renderSurface.draw(constellation);
	}
	if(currentMode == FADE_OUT)
	{
		int darkness = (int) (255 * elapsedTime.asSeconds() / fadeOutTime.asSeconds());
		constellation.setColor(sf::Color(255,255,255,255 - darkness));
		renderSurface.draw(constellation);
	}
}

void ConstellationOverlay::swapMode(Mode mode)
{
	currentMode = mode;
	elapsedTime = sf::seconds(0.0f);
}