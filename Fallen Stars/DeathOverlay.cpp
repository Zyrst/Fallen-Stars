#include "DeathOverlay.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "BaseResolution.h"
#include "PlatformState.h"

#include <iostream> // TODO Remove!

const sf::Time fadeOutTime = sf::seconds(2.0f);
const sf::Time blinkTime = sf::seconds(1.5f);
const sf::Time swarmTime = sf::seconds(3.0f);
const sf::Time darkTime = sf::seconds(1.0f);
const sf::Time fadeInTime = sf::seconds(2.0f);

DeathOverlay::DeathOverlay(int id, ResourceCollection& resources, PlatformState& platformState):
	Overlay(id, false),
	currentMode(FADE_OUT),
	platformState(platformState)
{
	//std::cout << "Create Death!" << std::endl;
}

void swapMode(DeathOverlay::Mode mode);

void DeathOverlay::update(const sf::Time& deltaTime)
{
	elapsedTime += deltaTime;

	//std::cout << "Draw!" << std::endl;

	if(currentMode == FADE_OUT)
	{
		if(elapsedTime > fadeOutTime)
		{
			swapMode(DARK);
			platformState.reset(); // Reset the game
		}
	}
	else if(currentMode == BLINK) // TODO Actually use this
	{
		if(elapsedTime > blinkTime) swapMode(SWARMED);
	}
	else if(currentMode == SWARMED) // TODO Actually use this
	{
		if(elapsedTime > swarmTime)
		{
			swapMode(DARK);
			platformState.reset(); // Reset the game
		}
	}
	else if(currentMode == DARK)
	{
		if(elapsedTime > darkTime) swapMode(FADE_IN);
	}
	else if(currentMode == FADE_IN)
	{
		if(elapsedTime > fadeInTime) swapMode(DONE);
	}
	else // currentMode == DONE
	{	
		// Restore the death screen to its initial state and hide it so that it doesn't show up again
		swapMode(FADE_OUT);
		setEnabledState(false);
	}
}

void DeathOverlay::render(sf::RenderTarget& renderSurface)
{
	

	if(currentMode == FADE_OUT)
	{
		int brightness = (int) (255 * elapsedTime.asSeconds() / fadeOutTime.asSeconds());

		sf::RectangleShape screenDimmer(Resolution::currentSimulatedResolution);
		screenDimmer.setFillColor(sf::Color(0,0,0,brightness));
		screenDimmer.setPosition(renderSurface.getView().getCenter() - Resolution::currentSimulatedResolution/2.0f);
		renderSurface.draw(screenDimmer);
	}
	else if(currentMode == BLINK) // TODO Actually use this
	{
		
	}
	else if(currentMode == SWARMED) // TODO Actually use this
	{
		
	}
	else if(currentMode == DARK)
	{
		renderSurface.clear();
	}
	else if(currentMode == FADE_IN)
	{
		int darkness = (int) (255 * elapsedTime.asSeconds() / fadeOutTime.asSeconds());

		sf::RectangleShape screenDimmer(Resolution::currentSimulatedResolution);
		screenDimmer.setFillColor(sf::Color(0,0,0, 255 - darkness));
		screenDimmer.setPosition(renderSurface.getView().getCenter() - Resolution::currentSimulatedResolution/2.0f);
		renderSurface.draw(screenDimmer);
	}
}

void DeathOverlay::reset()
{
	
}

void DeathOverlay::swapMode(Mode mode)
{
	elapsedTime = sf::seconds(0.0f);
	currentMode = mode;
}