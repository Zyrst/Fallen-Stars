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
	mLevel->getSoundLayer(mMusicVector, mResourceCollection);
	

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

	mListener.setPosition(mPlayer->getPosition().x,mPlayer->getPosition().y,0);

	for (auto i = 0; i < mMusicVector.size(); i++)
	{

		auto x= (mMusicVector[i]->getPosition().x - mListener.getPosition().x) * (mMusicVector[i]->getPosition().x - mListener.getPosition().x);
		auto y = (mMusicVector[i]->getPosition().y - mListener.getPosition().y) * (mMusicVector[i]->getPosition().y - mListener.getPosition().y);
		auto distance = std::sqrtf(x + y);
		std::cout << distance << std::endl;

		if (distance > 1000 )
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