#include "PlatformState.h"
#include "LightSolver.h"

#include "Game.h"
#include "MainMenuState.h"

PlatformState::PlatformState(std::string levelname)
: mLevelName(levelname)
, mLightSolver(new LightSolver())
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
	mWorld = new BoxWorld(b2Vec2(0, 10));
	
	mLevel = new LevelManager(mLevelName, &mResourceCollection);
	mLevel->genCollision(mWorld, mLightSolver);

	/*StreetLight* light = new StreetLight(mWorld, mLightSolver, sf::Vector2f(8370, 508), sf::Vector2f(800, 1024), sf::FloatRect(8153, 436, 495, 969), &mResourceCollection.getTexture("Assets/Shader/streetlightmask.png"));
	mEntityVector.push_back(light);*/

	auto size = sf::Vector2f(70, 220);
	
	/*Adds player and objects to the Level*/
	auto playerPos = mLevel->getPlayerLayer();
	mPlayer = new Player(mWorld, size, playerPos, mResourceCollection, mLightSolver);
	mEntityVector.push_back(mPlayer);
	
	mLevel->getStarLayer(mResourceCollection,mWorld,mEntityVector);
	mLevel->getStarDustLayer(mResourceCollection,mWorld,mEntityVector);
	mLevel->getEnemyLayer(mResourceCollection,mWorld,mEntityVector,size);
	mLevel->getStreetlightLayer(mResourceCollection, mWorld, mLightSolver, mEntityVector);
	mLevel->getSoundLayer(mMusicVector);
	

	for (Entity* e : mEntityVector)
	{
		mLightSolver->addOccluder(e);
	}

	mFirstSong = new sf::Music;

	mFirstSong->openFromFile("Assets/Sound/" + mLevelName + ".ogg");
	mFirstSong->setLoop(false);
	mMusicVector.push_back(mFirstSong);
	

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

	for(unsigned int i = 0; i < mMusicVector.size(); i++)
	{
		if(mMusicVector[i]->getLoop() == false)
		{
			mMusicVector[i]->play();
			mMusicVector[i]->setLoop(true);
		}
	}
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

	mLevel->getMapLoader().Draw(window, tmx::MapLayer::Foreground);

	/*Remove this to remove the outdrawn collision boxes and other box2d stuff*/
	//mWorld->drawDebug(window);
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

	for(unsigned int i = 0; i < mMusicVector.size(); i++)
	{
		//Stop the music before we erase it from the vector
		mMusicVector[i]->stop();
		mMusicVector[i]->setLoop(false);
	}

	for (auto it = mMusicVector.begin(); it != mMusicVector.end();)
	{
		it = mMusicVector.erase(it);
	}
}