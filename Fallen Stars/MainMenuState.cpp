#include "MainMenuState.h"


void MainMenuState::load()
{
	addOverlay(new MainMenu(MAIN_MENU, mResourceCollection));
}

void MainMenuState::update(const sf::Time& deltaTime)
{
}

void MainMenuState::render(sf::RenderWindow& window)
{
	renderOverlays(window);
}

void MainMenuState::handleAction(Controls::Action action, Controls::KeyState keystate)
{
	handleOverlayAction(action, keystate);
}