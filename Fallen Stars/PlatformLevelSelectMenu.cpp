#include "PlatformLevelSelectMenu.h"

#include "Button.h"
#include "BaseResolution.h"
#include "MainMenuState.h"
#include "PlatformState.h"
#include "Game.h"


PlatformLevelSelectMenu::PlatformLevelSelectMenu(int id, ResourceCollection& resources, const MainMenuState* state):
	Menu(id, resources, false),
	mLevels(LevelTracker::getPlatformLevels()),
	mState(const_cast<MainMenuState*>(state))
{
	sf::Vector2f width = sf::Vector2f((float)baseWidth, 0.0f);
	sf::Vector2f height = sf::Vector2f(0.0f, (float)baseHeight);

	sf::Texture& buttonTexture = resources.getTexture("Assets/Menu/Button.png");
	sf::Font& font = resources.getFont("Assets/Menu/24Janvier.otf");
	
	for(unsigned int i = 0; i < mLevels.size(); i++)
	{
		sf::Text text(mLevels[i].filename, font, 40U);
		sf::Vector2f offset = height / 2.0f * (float) i / (float) mLevels.size();
		addButton(Button((int)i, width / 2.0f + height / 4.0f + offset, buttonTexture, resources, text));
	}
	
	sf::Text back("Back", font, 40U);
	addButton(Button(BACK, width / 2.0f + height * 3.0f / 4.0f, buttonTexture, resources, back));
}

void PlatformLevelSelectMenu::buttonPressed(int id)
{
	if(id == Buttons::BACK)
	{
		setEnabled(false);
		mState->getOverlay(MainMenuState::MAIN_MENU).setEnabled(true);
	}

	if(id >= 0 && id < (int)mLevels.size())
	{
		Game::instance()->loadNewState(new PlatformState(mLevels[id].filename));
	}
}