#pragma once

#include "Menu.h"
#include "LevelTracker.h"

class MainMenuState;
class Game;
class ResourceCollection;

class PlatformLevelSelectMenu : public Menu
{
public:
	PlatformLevelSelectMenu(int id, ResourceCollection& resources, const MainMenuState* state);
	void buttonPressed(int id);

private:
	enum Buttons {BACK = 100};
	std::vector<PlatformData> mLevels;
	MainMenuState* mState;
};

