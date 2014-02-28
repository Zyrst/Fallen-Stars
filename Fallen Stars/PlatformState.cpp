#include "PlatformState.h"
#include "LightSolver.h"

#include "Game.h"
#include "MainMenuState.h"

PlatformState::PlatformState(std::string levelname):
	mLevelName(levelname)
{
}

PlatformState::~PlatformState()
{
	delete mWorld;
	delete mLevel;
	delete mCamera;
	delete mLightSolver;
	clear();
}

void PlatformState::load()
{
	mLightSolver = new LightSolver();

	mWorld = new BoxWorld(b2Vec2(0, 10));
	
	mLevel = new LevelManager(mLevelName);
	mLevel->genCollision(mWorld, mLightSolver);
	
	auto size = sf::Vector2f(70, 220);
	
	/*Adds player and objects to the Level*/
	auto playerPos = mLevel->getPlayerLayer();
	mPlayer = new Player(mWorld, size, playerPos, mResourceCollection, mLightSolver);
	mEntityVector.push_back(mPlayer);
	
	mLevel->getStarLayer(mResourceCollection,mWorld,mEntityVector);
	mLevel->getStarDustLayer(mResourceCollection,mWorld,mEntityVector);
	mLevel->getEnemyLayer(mResourceCollection,mWorld,mEntityVector,size);

	for (Entity* e : mEntityVector)
	{
		mLightSolver->addOccluder(e);
	}
//	mFirstSong.openFromFile("Assets/Sound/" + mLevelName + ".ogg");
	
	//mFirstSong.setLoop(true);

	sf::Vector2u mapSize =  mLevel->getMapLoader().GetMapSize();
	mCamera = new Camera(mPlayer, mapSize);
}

void PlatformState::update(const sf::Time& deltaTime)
{
	mWorld->step(deltaTime.asSeconds());
	for(unsigned int i = 0; i < mEntityVector.size(); i++)
	{
		mEntityVector[i]->update(deltaTime);
	}
	killDeadEntities();
	
/*	if (mFirstSong.getLoop() == false)
	{
		mFirstSong.play();
		mFirstSong.setLoop(true);
	}*/

}
void PlatformState::render(sf::RenderWindow& window)
{
	mCamera->update(window);
	mLevel->getMapLoader().Draw(window, tmx::MapLayer::Background);

	mLightSolver->render(window);

	for(unsigned int i = 0; i< mEntityVector.size();i++)
	{
		mEntityVector[i]->render(window);
	}

	//mLevel->getMapLoader().Draw(window, tmx::MapLayer::Foreground);

	/*Remove this to remove the outdrawn collision boxes and other box2d stuff*/
	mWorld->drawDebug(window);
}

void PlatformState::handleAction(Controls::Action action, Controls::KeyState keystate)
{
	mPlayer->handleAction(action, keystate);
	if(keystate == Controls::RELEASED && action == Controls::MENU) Game::instance()->loadNewState(new MainMenuState());
}

void PlatformState::killDeadEntities()
{
	for (auto i = mEntityVector.begin(); i < mEntityVector.end();)
	{
		if(!(*i)->isAlive())
		{
			mLightSolver->removeOccluder(*i);
			delete *i;
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