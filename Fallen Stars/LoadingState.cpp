#include "LoadingState.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <thread>
#include <cassert>
#include <cmath>
#include <iostream>

#include "Game.h"

ResourceCollection LoadingState::mPermanentResources = ResourceCollection();

LoadingState::LoadingState(State* nextState):
	mState(nextState),
	mProgress(NOT_LOADED)
{
	assert(nextState != NULL);
}

void LoadingState::load()
{
	// TODO Load loading background
	sf::Texture& texture = mPermanentResources.getTexture("Assets/Menu/Loading.png");
	sprite.setTexture(texture);
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
		std::cout << "Loading thread stopped. (Join successful)" << std::endl;
		
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
	window.clear(sf::Color(0, 0, 0, 255));
	window.draw(sprite);
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

std::string LoadingState::getTypeName()
{
	return "Loading State";
}