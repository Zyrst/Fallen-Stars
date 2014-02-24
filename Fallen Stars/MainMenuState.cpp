#include "MainMenuState.h"

void MainMenuState::load()
{
	addOverlay(new MainMenu(MAIN_MENU, mResourceCollection));
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
	renderOverlays(window);
}

void MainMenuState::handleAction(Controls::Action action, Controls::KeyState keystate)
{
	handleOverlayAction(action, keystate);
}