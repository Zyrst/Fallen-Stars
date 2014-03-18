#pragma once
#include <vector>
#include "State.h"
#include <SFML/Audio/Music.hpp>

class LightSolver;
class Player;
class Entity;
struct StatManager;
class ControlMapping;
class BoxWorld;
class Camera;
class LevelManager;

class PlatformState: public State
{
public:
	enum Overlays { DEATH_SCREEN, HUD, DIALOGUE, FADE_IN, FADE_OUT };

	PlatformState(std::string);
	~PlatformState();
	void update(const sf::Time& deltaTime) override;
	void render(sf::RenderWindow& window) override;
	void handleAction(Controls::Action action, Controls::KeyState keystate) override;
	void clear();
	void killDeadEntities();
	void load();
	void reset();
	void pauseGame();
	void resumeGame();

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
	StatManager* mStats;

	bool mDrawDebugShapes;
	bool mPaused;
};
