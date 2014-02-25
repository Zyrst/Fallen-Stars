#include "MainMenu.h"

#include "Button.h"
#include "BaseResolution.h"
#include "Game.h"
#include "JumpingTest.h"
#include "PuzzleState.h"
#include "MainMenuState.h"

#include <iostream> // TODO Remove!

MainMenu::MainMenu(int id, ResourceCollection& resources, const MainMenuState* state):
	Menu(id),
	mState(const_cast<MainMenuState*>(state))
{
	sf::Vector2f width = sf::Vector2f((float)baseWidth, 0.0f);
	sf::Vector2f height = sf::Vector2f(0.0f, (float)baseHeight);

	sf::Texture& buttonTexture = resources.getTexture("Assets/Menu/Button.png");
	sf::Font& font = resources.getFont("Assets/Menu/24Janvier.otf");
	
	sf::Text start("PlatformState", font, 30U);
	addButton(Button(START, width / 2.0f + height / 3.0f, buttonTexture, resources, start));
	
	sf::Text puzzle("PuzzleState", font, 30U);
	addButton(Button(PUZZLE, width / 2.0f + height / 2.0f, buttonTexture, resources, puzzle));
	
	sf::Text settings("Settings", font, 30U);
	addButton(Button(SETTINGS, width / 2.0f + height * 2.0f / 3.0f, buttonTexture, resources, settings));
	
}

void MainMenu::buttonPressed(int id)
{
	if(id == Buttons::START)
	{
		mState->getOverlay(MainMenuState::MAIN_MENU).setEnabledState(false);
		mState->getOverlay(MainMenuState::PLATFORM_SELECT).setEnabledState(true);
	}
	if(id == Buttons::PUZZLE)
	{
		mState->getOverlay(MainMenuState::MAIN_MENU).setEnabledState(false);
		mState->getOverlay(MainMenuState::PUZZLE_SELECT).setEnabledState(true);
	}
	if(id == Buttons::SETTINGS)
	{
		std::cout << "Settings! Yay ^^ (There aren't any yet)" << std::endl;
	}
}