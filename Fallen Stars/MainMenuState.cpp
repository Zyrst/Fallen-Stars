#include "MainMenuState.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include "MainMenu.h"
#include "MainMenuDebug.h"
#include "PlatformLevelSelectMenu.h"
#include "PuzzleSelectMenu.h"

void MainMenuState::load()
{
	mBackground.setTexture(mResourceCollection.getTexture("Assets/Menu/Main Menu.png"));

	addOverlay(new MainMenu(MAIN_MENU, mResourceCollection, this));
	addOverlay(new MainMenuDebug(MAIN_MENU_DEBUG, mResourceCollection, this));
	addOverlay(new PlatformLevelSelectMenu(PLATFORM_SELECT, mResourceCollection, this));
	addOverlay(new PuzzleSelectMenu(PUZZLE_SELECT, mResourceCollection, this));

	mMusic.openFromFile("Assets/Sound/MainMenuMusic.ogg");
	mMusic.setLoop(false);
}

void MainMenuState::update(const sf::Time& deltaTime)
{
	if (mMusic.getLoop() == false)
	{
		mMusic.play();
		mMusic.setLoop(true);
	}
}

void MainMenuState::render(sf::RenderWindow& window)
{
	window.draw(mBackground);
	renderOverlays(window);
}

void MainMenuState::handleAction(Controls::Action action, Controls::KeyState keystate)
{
	handleOverlayAction(action, keystate);
}