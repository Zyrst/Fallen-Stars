#include "PlatformState.h"
#include "LightSolver.h"

#include "Game.h"
#include "MainMenuState.h"

PlatformState::PlatformState(std::string levelname)
:	mLevelName(levelname)
,	mLightSolver(new LightSolver())
,	drawDebugShapes(true)
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
	
	mStats = new StatManager();

	auto size = sf::Vector2f(70, 220);
	
	/*Adds player and objects to the Level*/
	auto playerPos = mLevel->getPlayerLayer();
	mPlayer = new Player(mWorld, size, playerPos, mResourceCollection, mLightSolver);
	mEntityVector.push_back(mPlayer);
	mLevel->getObjectLayer(&mResourceCollection,mWorld,mEntityVector,mStats);
	mLevel->getEnemyLayer(mResourceCollection,mWorld,mEntityVector,size);
	mLevel->getSoundLayer(mMusicVector, mResourceCollection);
	mLevel->getSiriusLayer(*this, mResourceCollection);
	mLevel->getStreetlightLayer(mResourceCollection, mWorld, mLightSolver, mEntityVector);

	for (Entity* e : mEntityVector)
	{
		mLightSolver->addOccluder(e);
	}

	/*Level 'theme' music*/
	mFirstSong = new sf::Music;
	mFirstSong->openFromFile("Assets/Sound/" + mLevelName + ".ogg");
	mFirstSong->setLoop(false);
	mFirstSong->setVolume(100);
	
	mListener.setPosition(mPlayer->getPosition().x,mPlayer->getPosition().y,0);

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

	//Background sounds/music
	for(unsigned int i = 0; i < mMusicVector.size(); i++)
	{
		if(mMusicVector[i]->getLoop() == false)
		{
			mMusicVector[i]->play();
			mMusicVector[i]->setLoop(true);
		}
	}
	
	if(mFirstSong->getLoop() == false)
	{
		mFirstSong->play();
		mFirstSong->setLoop(true);
	}

	mListener.setPosition(mPlayer->getPosition().x, mPlayer->getPosition().y,0);

	//Positional sound and music
	for (auto i = 0; i < mMusicVector.size(); i++)
	{
		float x = (mMusicVector[i]->getPosition().x - mListener.getPosition().x) * (mMusicVector[i]->getPosition().x - mListener.getPosition().x);
		float y = (mMusicVector[i]->getPosition().y - mListener.getPosition().y) * (mMusicVector[i]->getPosition().y - mListener.getPosition().y);
		float distance = std::sqrtf(x + y);

	/*	auto volume = 1500 / (distance+100);
		mMusicVector[i]->setVolume(volume);*/
		if (distance > 1000)
		{
			mMusicVector[i]->setVolume(0);
		}
		if (distance <= 200)
		{
			mMusicVector[i]->setVolume(20);
		}
		if (distance >200 && distance <= 500 )
		{
			mMusicVector[i]->setVolume(15);
		}
		if (distance > 501 && distance <=1000)
		{
			mMusicVector[i]->setVolume(5);
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
	if(drawDebugShapes) mWorld->drawDebug(window);
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

	//Stop and then erase the level music
	mFirstSong->stop();
	mFirstSong->setLoop(false);
	delete mFirstSong;

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