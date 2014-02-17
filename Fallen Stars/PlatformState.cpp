#include "PlatformState.h"

PlatformState::PlatformState()
{
}

PlatformState::~PlatformState()
{
	delete mWorld;
	delete mLevel;
	delete mCamera;
	clear();
}

void PlatformState::load()
{
	mWorld = new BoxWorld(b2Vec2(0, 10));
	mLevel = new LevelManager("Test");
	mLevel->genCollision(mWorld);
	
	auto size = sf::Vector2f(70, 220);
	
	/*Adds player and objects to the Level*/
	auto playerPos = mLevel->getPlayerLayer();
	mPlayer = new Player(mWorld, size, playerPos,mResourceCollection);
	mEntityVector.push_back(mPlayer);
	
	mLevel->getStarLayer(mResourceCollection,mWorld,mEntityVector);
	mLevel->getStarDustLayer(mResourceCollection,mWorld,mEntityVector);
	mLevel->getEnemyLayer(mResourceCollection,mWorld,mEntityVector,size);

	sf::Vector2u mapSize =  mLevel->getMapLoader().GetMapSize();
	mCamera = new Camera(mPlayer, mapSize);
}

void PlatformState::update(const sf::Time& deltaTime)
{
	killDeadEntities();
	for(unsigned int i = 0; i< mEntityVector.size();i++)
	{
		mEntityVector[i]->update(deltaTime);
	}
	mWorld->step(deltaTime.asSeconds());
}
void PlatformState::render(sf::RenderWindow& window)
{
	mCamera->update(window);
	mLevel->getMapLoader().Draw(window, tmx::MapLayer::Background);
	for(unsigned int i = 0; i< mEntityVector.size();i++)
	{
		mEntityVector[i]->render(window);
	}
	mLevel->getMapLoader().Draw(window, tmx::MapLayer::Foreground);
	mWorld->drawDebug(window);
}

void PlatformState::handleAction(Controls::Action action, Controls::KeyState keystate)
{
	mPlayer->handleAction(action, keystate);
}

void PlatformState::killDeadEntities()
{
	for (auto i = mEntityVector.begin(); i < mEntityVector.end();)
	{
		if(!(*i)->isAlive())
		{
			delete *i; //?
			i = mEntityVector.erase(i);
			std::cout << "Erased entity" << std::endl;
		}
		else 
			i++;
	}

}

void PlatformState::clear()
{
	for (auto it = mEntityVector.begin(); it != mEntityVector.end();)
	{
		it = mEntityVector.erase(it);
	}
}