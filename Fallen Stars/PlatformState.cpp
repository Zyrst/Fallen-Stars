#include "PlatformState.h"

#include "LightSolver.h"
#include "Game.h"
#include "MainMenuState.h"
#include "DeathOverlay.h"
#include "DialogueOverlay.h"
#include "HUDOverlay.h"
#include "LevelFadeOut.h"
#include "LevelFadeIn.h"
#include "Entity.h"
#include "Object.h"
#include "Player.h"
#include "LevelManager.h"
#include "StatManager.h"
#include "ControlMapping.h"
#include "BoxWorld.h"
#include "Camera.h"
#include "LevelManager.h"
#include "PauseMenu.h"

PlatformState::PlatformState(std::string levelname)
:	mLevelName(levelname)
,	mLightSolver(new LightSolver)
,	mDrawDebugShapes(false)
,	mPaused(false)
,	mStats(NULL)
,	mCamera(NULL)
{
}

PlatformState::~PlatformState()
{
	//Stop and then erase the level music
	mFirstSong->stop();
	mFirstSong->setLoop(false);
	mAmbience->stop();
	mAmbience->setLoop(false);
	for(unsigned int i = 0; i < mMusicVector.size(); i++)
	{
		//Stop the music before we erase it from the vector
		mMusicVector[i]->stop();
		mMusicVector[i]->setLoop(false);
	}
	for (sf::Music* m : mMusicVector) delete m;
	for (AnimatedBackgroundImage* a : mAnimations) delete a;
	mMusicVector.clear();
	mAnimations.clear();
	clear();
	delete mFirstSong;
	delete mAmbience;
	delete mWorld;
	delete mLevel;
	delete mLightSolver;
	delete mStats;
}

void PlatformState::load()
{
	mWorld = new BoxWorld(b2Vec2(0, 10));
	
	mLevel = new LevelManager(mLevelName, &mResourceCollection);
	mLevel->genCollision(mWorld, mLightSolver);
	
	mStats = new StatManager;

	addOverlay(new DialogueOverlay(DIALOGUE, mResourceCollection, *mWorld, *this));

	reset();
	mLevel->getSoundLayer(mMusicVector, mResourceCollection);
	mLevel->getAnimationLayer(mAnimations, mResourceCollection);

	/*Level 'theme' music*/
	mFirstSong = new sf::Music;
	mFirstSong->openFromFile("Assets/Sound/" + mLevelName + ".ogg");
	mFirstSong->setLoop(false);
	mFirstSong->setVolume(100);

	/*Ambience sound*/
	mAmbience = new sf::Music;
	mAmbience->openFromFile("Assets/Sound/" + mLevelName + "_ambience.ogg");
	mAmbience->setLoop(false);

	addOverlay(new DeathOverlay(DEATH_SCREEN, mResourceCollection, *this));
	addOverlay(new HUDOverlay(HUD, *mStats, mResourceCollection));
	addOverlay(new PauseMenu(PAUSE, mResourceCollection, this));
	addOverlay(new LevelFadeIn(FADE_IN));
	addOverlay(new LevelFadeOut(FADE_OUT));
}

void PlatformState::update(const sf::Time& deltaTime)
{
	updateOverlays(deltaTime);

	if(mPaused) return;

	mWorld->step(deltaTime.asSeconds());
	for(unsigned int i = 0; i < mEntityVector.size(); i++)
	{
		mEntityVector[i]->update(deltaTime);
	}
	killDeadEntities();

	if(mPlayer->getPosition().y > 4000) getOverlay(DEATH_SCREEN).setEnabled(true);

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

	if(mAmbience->getLoop() == false)
	{
		mAmbience->play();
		mAmbience->setLoop(true);
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

	for (AnimatedBackgroundImage* image : mAnimations) image->update(deltaTime);

	// Swap level when all stars are collected
	if(mStats->stars == mStats->totalStars) getOverlay(FADE_OUT).setEnabled(true);
}
void PlatformState::render(sf::RenderWindow& window)
{
	mCamera->update(window);
	mLevel->getMapLoader().Draw(window, tmx::MapLayer::Background);

	for (AnimatedBackgroundImage* image : mAnimations) image->render(window);

	for(unsigned int i = 0; i< mEntityVector.size();i++)
	{
		if(dynamic_cast<Object*>(mEntityVector[i]) == NULL) // Not an object
		{
			mEntityVector[i]->render(window);
		}
	}

	mLightSolver->render(window);

	for(unsigned int i = 0; i< mEntityVector.size();i++)
	{
		if(dynamic_cast<Object*>(mEntityVector[i]) != NULL) // Is an object
		{
			mEntityVector[i]->render(window);
		}
	}

	mLevel->getMapLoader().Draw(window, tmx::MapLayer::Foreground);

	/*Remove this to remove the outdrawn collision boxes and other box2d stuff*/
	if(mDrawDebugShapes) mWorld->drawDebug(window);

	renderOverlays(window);
}

void handleResize(int width, int height) 
{

}

void PlatformState::handleAction(Controls::Action action, Controls::KeyState keystate)
{
	handleOverlayAction(action, keystate);

	if(mPaused) return;

	mPlayer->handleAction(action, keystate);
	if(keystate == Controls::RELEASED)
	{
		if(action == Controls::MENU)
		{
			getOverlay(PAUSE).setEnabled(true);
			pauseGame();
		}
		if(action == Controls::DEBUG) mDrawDebugShapes = !mDrawDebugShapes; 
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
	if(mCamera != NULL) delete mCamera;
}

void PlatformState::reset()
{
	/*Clears out vectors so we can add new elements*/
	clear();
	*mStats = StatManager();

	/*Add Player and the other elements to the level*/
	auto size = sf::Vector2f(70, 220);
	auto playerPos = mLevel->getPlayerLayer();
	mLevel->getObjectLayer(&mResourceCollection,mWorld,mEntityVector,mStats);
	mLevel->getEnemyLayer(mResourceCollection,mWorld,mEntityVector,size);
	mLevel->getStreetlightLayer(mResourceCollection, mWorld, mLightSolver, mEntityVector);
	mPlayer = new Player(*this, mWorld, size, playerPos, mResourceCollection, mLightSolver,*mStats);
	mEntityVector.push_back(mPlayer);
	mLevel->getDialogueLayer(*this, mResourceCollection);

	for (Entity* e : mEntityVector)
	{
		mLightSolver->addOccluder(e);
	}

	sf::Vector2u mapSize = mLevel->getMapLoader().GetMapSize();
	mCamera = new Camera(mPlayer, mapSize);
}

void PlatformState::pauseGame()
{
	mPaused = true;
}

void PlatformState::resumeGame()
{
	mPaused = false;
}

std::string PlatformState::getTypeName()
{
	return "Platform State";
}