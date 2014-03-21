#include "MainMenuDebug.h"

#include "Button.h"
#include "BaseResolution.h"
#include "Game.h"
#include "PuzzleState.h"
#include "MainMenuState.h"
#include "PlatformState.h"

#include <iostream> // TODO Remove!

MainMenuDebug::MainMenuDebug(int id, ResourceCollection& resources, const MainMenuState* state):
	Menu(id, false),
	mState(const_cast<MainMenuState*>(state))
{
	sf::Vector2f width = sf::Vector2f((float)baseWidth, 0.0f);
	sf::Vector2f height = sf::Vector2f(0.0f, (float)baseHeight);

	sf::Texture& buttonTexture = resources.getTexture("Assets/Menu/Button.png");
	sf::Font& font = resources.getFont("Assets/Menu/24Janvier.otf");
	
	sf::Text start("PlatformState", font, 30U);
	addButton(Button(START, width / 2.0f + height / 4.0f, buttonTexture, resources, start));
	
	sf::Text puzzle("PuzzleState", font, 30U);
	addButton(Button(PUZZLE, width / 2.0f + height * (1/4.0f + 1/6.0f), buttonTexture, resources, puzzle));
	
	sf::Text settings("Toggle Fullscreen", font, 30U);
	addButton(Button(FULLSCREEN, width / 2.0f + height * (1/4.0f + 2/6.0f), buttonTexture, resources, settings));
		
	sf::Text exit("Exit", font, 30U);
	addButton(Button(EXIT, width / 2.0f + height * (1/4.0f + 3/6.0f), buttonTexture, resources, exit));
}


void MainMenuDebug::buttonPressed(int id)
{
	if(id == Buttons::START)
	{
		setEnabled(false);
		mState->getOverlay(MainMenuState::PLATFORM_SELECT).setEnabled(true);
	}
	if(id == Buttons::PUZZLE)
	{
		setEnabled(false);
		mState->getOverlay(MainMenuState::PUZZLE_SELECT).setEnabled(true);
	}
	if(id == Buttons::FULLSCREEN)
	{
		Game::instance()->toggleFullscreen();
	}
	if(id == Buttons::EXIT)
	{
		Game::instance()->exit();
	}
}