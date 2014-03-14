#include "PlatformState.h"
#include "LightSolver.h"

#include "Game.h"
#include "MainMenuState.h"
#include "DeathOverlay.h"

PlatformState::PlatformState(std::string levelname)
:	mLevelName(levelname)
,	drawDebugShapes(true)
,	mLightSolver(new LightSolver)
{
}

PlatformState::~PlatformState()
{
	//Stop and then erase the level music
	mFirstSong->stop();
	mFirstSong->setLoop(false);
	for(unsigned int i = 0; i < mMusicVector.size(); i++)
	{
		//Stop the music before we erase it from the vector
		mMusicVector[i]->stop();
		mMusicVector[i]->setLoop(false);
	}
	for (sf::Music* m : mMusicVector)
	{
		delete m;
	}
	mMusicVector.clear();
	clear();
	delete mFirstSong;
	delete mWorld;
	delete mLevel;
	delete mCamera;
	delete mLightSolver;
}

void PlatformState::load()
{
	mWorld = new BoxWorld(b2Vec2(0, 10));
	
	mLevel = new LevelManager(mLevelName, &mResourceCollection);
	mLevel->genCollision(mWorld, mLightSolver);
	
	reset();
	mLevel->getSoundLayer(mMusicVector, mResourceCollection);

	/*Level 'theme' music*/
	mFirstSong = new sf::Music;
	mFirstSong->openFromFile("Assets/Sound/" + mLevelName + ".ogg");
	mFirstSong->setLoop(false);
	mFirstSong->setVolume(100);
	
	sf::Vector2u mapSize =  mLevel->getMapLoader().GetMapSize();
	mCamera = new Camera(mPlayer, mapSize);

	addOverlay(new DeathOverlay(DEATH_SCREEN, mResourceCollection));
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


	//Positional sound and music
	for (auto i = 0; i < (int) mMusicVector.size(); i++)
	{
		float x = (mMusicVector[i]->getPosition().x - mPlayer->getPosition().x) * (mMusicVector[i]->getPosition().x - mPlayer->getPosition().x);
		float y = (mMusicVector[i]->getPosition().y - mPlayer->getPosition().y) * (mMusicVector[i]->getPosition().y - mPlayer->getPosition().y);
		float distance = std::sqrtf(x + y);

	/*	auto volume = 1500 / (distance+100);
		mMusicVector[i]->setVolume(volume);*/
		if (distance > 1000)
		{
			mMusicVector[i]->setVolume(0);
		}
		if (distance <= 400)
		{
			mMusicVector[i]->setVolume(20);
		}
		if (distance >401 && distance <= 700 )
		{
			mMusicVector[i]->setVolume(15);
		}
		if (distance > 701 && distance <=1000)
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
	if(keystate == Controls::RELEASED)
	{
		if(action == Controls::MENU) Game::instance()->loadNewState(new MainMenuState());		
		if(action == Controls::DEBUG) drawDebugShapes = !drawDebugShapes;
	}
}

void PlatformState::killDeadEntities()
{
	for (auto i = mEntityVector.begin(); i < mEntityVector.end();)
	{
		if(!(*i)->isAlive())
		{
			mLightSolver->removeOccluder(*i);
			(*i)->destroyBody();
			delete *i;
			i = mEntityVector.erase(i);
		}
		else 
			i++;
	}

}

void PlatformState::clear()
{
	for (Entity* e : mEntityVector)
	{
		delete e;
	}
	mEntityVector.clear();
	mLightSolver->clear();	
	delete mStats;
}

void PlatformState::reset()
{
	/*Clears out vectors so we can add new elements*/
	clear();
	mStats = new StatManager();

	/*Add Player and the other elements to the level*/
	auto size = sf::Vector2f(70, 220);
	auto playerPos = mLevel->getPlayerLayer();
	mLevel->getObjectLayer(&mResourceCollection,mWorld,mEntityVector,mStats);
	mLevel->getEnemyLayer(mResourceCollection,mWorld,mEntityVector,size);
	mLevel->getStreetlightLayer(mResourceCollection, mWorld, mLightSolver, mEntityVector);
	mPlayer = new Player(mWorld, size, playerPos, mResourceCollection, mLightSolver,*mStats);
	mEntityVector.push_back(mPlayer);
	mLevel->getSiriusLayer(*this, mResourceCollection);

	for (Entity* e : mEntityVector)
	{
		mLightSolver->addOccluder(e);
	}

}