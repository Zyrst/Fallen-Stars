#include "LevelFadeIn.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "Game.h"
#include "PuzzleState.h"
#include "BaseResolution.h"

const sf::Time fadeTime = sf::seconds(2.0);

LevelFadeIn::LevelFadeIn(int id):
	Overlay(id),
	elapsedTime(sf::seconds(0.0f))
{
}

void LevelFadeIn::update(const sf::Time& deltaTime)
{
	elapsedTime += deltaTime;

	if(elapsedTime >= fadeTime)
	{
		setEnabled(false);
	}
}

void LevelFadeIn::render(sf::RenderTarget& renderSurface)
{
	int darkness = (int) (255 * elapsedTime.asSeconds() / fadeTime.asSeconds());

	sf::RectangleShape screenDimmer(Resolution::currentSimulatedResolution);
	screenDimmer.setFillColor(sf::Color(0,0,0, 255 - darkness));
	screenDimmer.setPosition(renderSurface.getView().getCenter() - Resolution::currentSimulatedResolution/2.0f);
	renderSurface.draw(screenDimmer);
}