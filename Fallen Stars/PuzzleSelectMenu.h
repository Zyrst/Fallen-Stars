#pragma once

#include "Menu.h"
#include "LevelTracker.h"
#include "Game.h"

class MainMenuState;

class PuzzleSelectMenu : public Menu
{
public:
	PuzzleSelectMenu(int id, ResourceCollection& resources, const MainMenuState* state);
	void buttonPressed(int id);

private:
	enum Buttons {BACK = 100};
	std::vector<PuzzleData> mPuzzles;
	MainMenuState* mState;
};

