#include "PuzzleState.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <random>
#include <chrono>

#include "BaseResolution.h"
#include "MainMenuState.h"
#include "ConstellationOverlay.h"
#include "Game.h"


PuzzleState::PuzzleState(std::string level, int ringCount, int steps, sf::Vector2f center):
	mLevel(level),
	mRingCount(ringCount),
	mSteps(steps),
	mCenter(center),
	mSelectedRing(0),
	completed(false),
	mBackground()
{
}


PuzzleState::~PuzzleState()
{
	mMusic.setLoop(false);
	mMusic.stop();
}

void PuzzleState::load()
{
	addOverlay(new ConstellationOverlay(CONSTELLATION, mResourceCollection, mLevel));

	mBackground.setTexture(mResourceCollection.getTexture("Assets/Puzzle/" + mLevel + "/Background.png"));

	mMusic.openFromFile("Assets/Sound/" + mLevel + "_Puzzle_Music.ogg");
	mMusic.setLoop(false);

	std::random_device randomDevice;
	std::mt19937 twisterEngine(randomDevice());
	std::uniform_int_distribution<int> limit(0, mSteps - 1);

	for(int ringNum = 1; ringNum < mRingCount; ringNum++)
	{
		mRings.push_back(PuzzleRing(mResourceCollection, mLevel, ringNum, mCenter, limit(twisterEngine), mSteps));
	}
	mRings.push_back(PuzzleRing(mResourceCollection, mLevel, mRingCount, mCenter, 0, mSteps));

	mRings[0].setHighLighted(true);
}

void PuzzleState::update(const sf::Time& deltaTime)
{
	updateOverlays(deltaTime);

	if (mMusic.getLoop() == false)
	{
		mMusic.play();
		mMusic.setLoop(true);
	}

	for(PuzzleRing& ring : mRings)
	{
		ring.update(deltaTime);
	}

	bool correct = true;
	for(PuzzleRing& ring : mRings)
	{
		if(!ring.isCorrect()) correct = false;
	}
	if(correct)
	{
		completed = true;
		getOverlay(CONSTELLATION).setEnabled(true);
	}

}

void PuzzleState::render(sf::RenderWindow& window)
{
	window.draw(mBackground);
	for(PuzzleRing& ring : mRings)
	{
		ring.render(window);
	}

	renderOverlays(window);
}

void PuzzleState::handleAction(Controls::Action action, Controls::KeyState keystate)
{
	if(completed) return;

	if(keystate == Controls::KeyState::RELEASED)
	{
		if(action == Controls::Action::DOWN)
		{
			swapRing(1);
		}
		if(action == Controls::Action::UP)
		{
			swapRing(-1);
		}
		if(action == Controls::MENU) Game::instance()->loadNewState(new MainMenuState());
	}

	mRings[mSelectedRing].handleAction(action, keystate);
}

void PuzzleState::swapRing(int indexOffset)
{
	const int availableRings = mRingCount - 1;
	mRings[mSelectedRing].setHighLighted(false);
	mSelectedRing = (mSelectedRing + indexOffset + availableRings) % availableRings;
	mRings[mSelectedRing].setHighLighted(true);
}