#include "PlatformState.h"


PlatformState::PlatformState()
{
}


PlatformState::~PlatformState()
{
}

void PlatformState::update(sf::Time deltaTime)
{
	for(int i = 0; i< mEntityVector.size();i++)
	{
		mEntityVector[i]->update(deltaTime);
	}
}
void PlatformState::render(sf::RenderWindow& window)
{
	
	for(int i = 0; i< mEntityVector.size();i++)
	{
		mEntityVector[i]->render(window);
	}
}

void PlatformState::handleAction(Controls::Action action, Controls::KeyState keystate)
{
	for(int i = 0; i< mEntityVector.size();i++)
	{
		mEntityVector[i]->handleAction(action, keystate);
	}
}

void PlatformState::clear()
{
	mEntityVector.clear();
}

void PlatformState::killDeadEntities()
{
	for (auto i = mEntityVector.begin(); i < mEntityVector.end();)
	{
		if(!(*i)->isAlive())
		{
			i = mEntityVector.erase(i);
		}
		else 
			i++;

	}

}