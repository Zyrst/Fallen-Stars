#pragma once

#include "Menu.h"

class MainMenuState;
class ResourceCollection;

class MainMenu : public Menu
{
public:
	MainMenu(int id, ResourceCollection& resources, const MainMenuState* state);
	void buttonPressed(int id);

private:
	enum Buttons {START, PUZZLE, FULLSCREEN, EXIT};
	MainMenuState* mState;
};