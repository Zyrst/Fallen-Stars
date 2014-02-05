#include "LoadingState.h"

#include <thread>
#include <cassert>
#include <cmath>
#include <iostream>

#include "Game.h"

LoadingState::LoadingState(State* nextState):
	mState(nextState),
	mProgress(NOT_LOADED)
{
	assert(nextState != NULL);
}

void LoadingState::load()
{
	// TODO Load loading background
}

void loadStateResources(LoadingState* loadingState)
{
	// Load all resources
	loadingState->getNextState()->load();

	// When done, set a flag to swap to the loaded state
	loadingState->setProgress(LoadingState::DONE_LOADING);
}

void LoadingState::update(sf::Time deltaTime)
{
	// Start off the loading
	if(mProgress == LoadingState::NOT_LOADED)
	{
		mLoadingThread = new std::thread(loadStateResources, this);
		setProgress(LoadingState::LOADING);
	}
	
	// Check if the loading is complete. If so, swap the state.
	if(mProgress == LoadingState::DONE_LOADING)
	{
		std::cout << "Thread about to join" << std::endl;
		mLoadingThread->join();
		std::cout << "Join successful" << std::endl;
		delete mLoadingThread;
		std::cout << "About to set a new state" << std::endl;
		Game::instance()->setState(mState);
		std::cout << "Setting new state successful";
	}

	// Update effects of the loading screen?
	mTime += deltaTime;
}

void LoadingState::render(sf::RenderWindow& window)
{
	// Display loading screen

	// TODO Replace this!
	/*float animationTime = 2;
	float sineWave = (float) sin(mTime.asSeconds() / animationTime * (2.0*3.141592));
	int brightness = (int) (127.5 + sineWave * 127.5);
	*/

	window.clear(sf::Color(0, 0, 255, 255));
}

void LoadingState::handleAction(Controls::Action action, Controls::KeyState) 
{}

void LoadingState::setProgress(LoadProgress progress)
{
	mProgress = progress;
}


State* LoadingState::getNextState()
{
	return mState;
}