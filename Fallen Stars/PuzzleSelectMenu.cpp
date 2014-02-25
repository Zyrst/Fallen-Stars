#include "PuzzleSelectMenu.h"

#include "Button.h"
#include "BaseResolution.h"
#include "MainMenuState.h"
#include "PuzzleState.h"

PuzzleSelectMenu::PuzzleSelectMenu(int id, ResourceCollection& resources, const MainMenuState* state):
	Menu(id, false),
	mPuzzles(LevelTracker::getPuzzles()),
	mState(const_cast<MainMenuState*>(state))
{
	sf::Vector2f width = sf::Vector2f((float)baseWidth, 0.0f);
	sf::Vector2f height = sf::Vector2f(0.0f, (float)baseHeight);

	sf::Texture& buttonTexture = resources.getTexture("Assets/Menu/Button.png");
	sf::Font& font = resources.getFont("Assets/Menu/24Janvier.otf");
	
	for(int i = 0; i < mPuzzles.size(); i++)
	{
		sf::Text text(mPuzzles[i].foldername, font, 30U);
		sf::Vector2f offset = height / 2.0f * (float) i / (float) mPuzzles.size();
		addButton(Button(i, width / 2.0f + height / 4.0f + offset, buttonTexture, resources, text));
	}
	
	sf::Text back("Back", font, 30U);
	addButton(Button(BACK, width / 2.0f + height * 3.0f / 4.0f, buttonTexture, resources, back));
}

void PuzzleSelectMenu::buttonPressed(int id)
{
	if(id == Buttons::BACK)
	{
		mState->getOverlay(MainMenuState::PUZZLE_SELECT).setEnabledState(false);
		mState->getOverlay(MainMenuState::MAIN_MENU).setEnabledState(true);
	}

	if(id >= 0 && id < mPuzzles.size())
	{
		Game::instance()->loadNewState(new PuzzleState(mPuzzles[id].foldername, mPuzzles[id].rings, 17));
	}
}