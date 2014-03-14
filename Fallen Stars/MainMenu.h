#pragma once

#include "Menu.h"

class MainMenuState;
class ResourceCollection;

class MainMenu : public Menu
{
public:
	MainMenu(int id, ResourceCollection& resources, const MainMenuState* state);
	void buttonPressed(int id) override;
	void handleAction(Controls::Action action, Controls::KeyState keystate) override;

private:
	enum Buttons {CONTINUE, NEW_GAME, FULLSCREEN, EXIT};
	MainMenuState* mState;
};

