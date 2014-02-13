#include "MainMenuState.h"


MainMenuState::~MainMenuState()
{
	delete menu;
}


void MainMenuState::load()
{
	menu = new MainMenu(mResourceCollection);
}

void MainMenuState::update(const sf::Time& deltaTime)
{
}

void MainMenuState::render(sf::RenderWindow& window)
{
	menu->render(window);
}

void MainMenuState::handleAction(Controls::Action action, Controls::KeyState keystate)
{
	menu->handleAction(action, keystate);
}