#include "MainMenu.h"

#include "Button.h"
#include "BaseResolution.h"
#include "Game.h"
#include "PuzzleState.h"
#include "MainMenuState.h"
#include "PlatformState.h"
#include "CutsceneState.h"

#include <iostream> // TODO Remove!

MainMenu::MainMenu(int id, ResourceCollection& resources, const MainMenuState* state) :
	Menu(id, resources),
	mState(const_cast<MainMenuState*>(state))
{
	sf::Vector2f width = sf::Vector2f((float)baseWidth, 0.0f);
	sf::Vector2f height = sf::Vector2f(0.0f, (float)baseHeight);

	sf::Texture& buttonTexture = resources.getTexture("Assets/Menu/Button.png");
	sf::Font& font = resources.getFont("Assets/Menu/24Janvier.otf");
	
	sf::Text textContinue("Continue", font, 30U);
	addButton(Button(CONTINUE, width / 2.0f + height / 6.0f, buttonTexture, resources, textContinue, false));
	
	sf::Text textNewGame("New Game", font, 30U);
	addButton(Button(NEW_GAME, width / 2.0f + height * (1/6.0f + 1/6.0f), buttonTexture, resources, textNewGame));
	
	sf::Text textFullscreen("Toggle Fullscreen", font, 30U);
	addButton(Button(FULLSCREEN, width / 2.0f + height * (1/6.0f + 2/6.0f), buttonTexture, resources, textFullscreen));
		
	sf::Text textExit("Exit", font, 30U);
	addButton(Button(EXIT, width / 2.0f + height * (1/6.0f + 3/6.0f), buttonTexture, resources, textExit));

}


void MainMenu::buttonPressed(int id)
{
	if(id == Buttons::CONTINUE)
	{
		// TODO Load last save and start from where it left off
		std::cout << "WTF?! HOW IS THIS HAPPENING?! D: THE DEAD BUTTONS ARE INVADING, RUN FOR YOUR LIVES!" << std::endl;
	}
	if(id == Buttons::NEW_GAME) 
	{
		// TODO Clear any previous progress and load level 1
		State* level = new PlatformState("Level 1");
		#ifdef _DEBUG
			// Don't display cutscene, the library doesn't support debug!
			Game::instance()->loadNewState(level);
		#else
			// Release is fine, so show the cutscene! :)
			//Game::instance()->loadNewState(level);
			Game::instance()->loadNewState(new CutsceneState("Assets/Video/Cutscene.webm", level));
		#endif
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

void MainMenu::handleAction(Controls::Action action, Controls::KeyState keystate)
{
	Menu::handleAction(action, keystate);
	if(action == Controls::Action::DEBUG && keystate == Controls::KeyState::RELEASED)
	{
		setEnabled(false);
		mState->getOverlay(MainMenuState::MAIN_MENU_DEBUG).setEnabled(true);
	}
}