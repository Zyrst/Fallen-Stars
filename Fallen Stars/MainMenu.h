#pragma once

#include "Menu.h"
#include "ResourceCollection.h"

class MainMenuState;

class MainMenu : public Menu
{
public:
	MainMenu(int id, ResourceCollection& resources, const MainMenuState* state);
	void buttonPressed(int id);

private:
	enum Buttons {START, PUZZLE, SETTINGS};
	MainMenuState* mState;
};

