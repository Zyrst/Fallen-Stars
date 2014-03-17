#include "LevelFadeOut.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "Game.h"
#include "PuzzleState.h"
#include "BaseResolution.h"

const sf::Time fadeTime = sf::seconds(2.0);

LevelFadeOut::LevelFadeOut(int id):
	Overlay(id, false),
	elapsedTime(sf::seconds(0.0f))
{
}

void LevelFadeOut::update(const sf::Time& deltaTime)
{
	elapsedTime += deltaTime;

	if(elapsedTime >= fadeTime)
	{
		Game::instance()->loadNewState(new PuzzleState("Level1", 4, 17));
	}
}

void LevelFadeOut::render(sf::RenderTarget& renderSurface)
{
	int brightness = (int) (255 * elapsedTime.asSeconds() / fadeTime.asSeconds());

	sf::RectangleShape screenDimmer(Resolution::currentSimulatedResolution);
	screenDimmer.setFillColor(sf::Color(0,0,0,brightness));
	screenDimmer.setPosition(renderSurface.getView().getCenter() - Resolution::currentSimulatedResolution/2.0f);
	renderSurface.draw(screenDimmer);
}