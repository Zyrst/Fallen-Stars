#include "PuzzleState.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <random>
#include <chrono>

#include "BaseResolution.h"
#include "MainMenuState.h"
#include "Game.h"

PuzzleState::PuzzleState(std::string level, int ringCount, int steps):
	mLevel(level),
	mRingCount(ringCount),
	mSteps(steps),
	mSelectedRing(0),
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
	mBackground.setTexture(mResourceCollection.getTexture("Assets/Puzzle/" + mLevel + "Background.png"));

	mMusic.openFromFile("Assets/Sound/" + mLevel + "_Puzzle_Music.ogg");
	mMusic.setLoop(false);

	std::random_device randomDevice;
	std::mt19937 twisterEngine(randomDevice());
	std::uniform_int_distribution<int> limit(0, mSteps - 1);

	for(int ringNum = 1; ringNum < mRingCount; ringNum++)
	{
		mRings.push_back(PuzzleRing(mResourceCollection, mLevel, ringNum, baseResolution / 2.0f, limit(twisterEngine), mSteps));
	}
	mRings.push_back(PuzzleRing(mResourceCollection, mLevel, mRingCount, baseResolution / 2.0f, 0, mSteps));

	mRings[0].setHighLighted(true);
}

void PuzzleState::update(const sf::Time& deltaTime)
{
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
	if(correct) Game::instance()->loadNewState(new MainMenuState());

}

void PuzzleState::render(sf::RenderWindow& window)
{
	window.draw(mBackground);
	for(PuzzleRing& ring : mRings)
	{
		ring.render(window);
	}
}

void PuzzleState::handleAction(Controls::Action action, Controls::KeyState keystate)
{
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