#pragma once

#include "Menu.h"

class MainMenuState;
class ResourceCollection;

class MainMenuDebug : public Menu
{
public:
	MainMenuDebug(int id, ResourceCollection& resources, const MainMenuState* state);
	void buttonPressed(int id) override;

private:
	enum Buttons {START, PUZZLE, FULLSCREEN, EXIT};
	MainMenuState* mState;
};