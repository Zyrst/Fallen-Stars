#pragma once
#include "ControlMapping.h"
#include "Entity.h"
#include "Player.h"
#include <vector>
#include "State.h"
#include "Camera.h"
#include "LevelManager.h"
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/Listener.hpp>
#include "StatManager.h"

class LightSolver;

class PlatformState: public State
{
public:
	PlatformState(std::string);
	~PlatformState();
	void update(const sf::Time& deltaTime) override;
	void render(sf::RenderWindow& window) override;
	void handleAction(Controls::Action action, Controls::KeyState keystate) override;
	void clear();
	void killDeadEntities();
	void load();

private:
	std::vector <Entity*> mEntityVector;
	std::vector <sf::Music*> mMusicVector;
	Player* mPlayer;
	BoxWorld* mWorld;
	Camera* mCamera;
	LevelManager* mLevel;
	LightSolver* mLightSolver;
	std::string mLevelName;
	sf::Music* mFirstSong;
	sf::Listener mListener;
	StatManager* mStats;

	bool drawDebugShapes;
};
