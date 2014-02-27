#include "LoadingState.h"

#include <SFML/Graphics/RenderWindow.hpp>
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

void LoadingState::update(const sf::Time& deltaTime)
{
	// Start off the loading
	if(mProgress == LoadingState::NOT_LOADED)
	{
		setProgress(LoadingState::LOADING);
		mLoadingThread = new std::thread(loadStateResources, this);
	}
	
	// Check if the loading is complete. If so, swap the state.
	if(mProgress == LoadingState::DONE_LOADING)
	{
		std::cout << "Loading is complete. Loading thread will now join with the gameloop thread." << std::endl;
		mLoadingThread->join();
		std::cout << "Join successful! (Loading thread stopped)" << std::endl;
		
		delete mLoadingThread; 
		mLoadingThread = NULL;
		
		Game::instance()->setState(mState);
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