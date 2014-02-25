#pragma once
#include "ControlMapping.h"
#include "Entity.h"
#include "Player.h"
#include <vector>
#include "State.h"
#include "Camera.h"
#include "LevelManager.h"

class LightSolver;

class PlatformState: public State
{
public:
	PlatformState();
	~PlatformState();
	void update(const sf::Time& deltaTime) override;
	void render(sf::RenderWindow& window) override;
	void handleAction(Controls::Action action, Controls::KeyState keystate) override;
	void clear();
	void killDeadEntities();
	void load();

private:
	std::vector <Entity*> mEntityVector;
	Player* mPlayer;
	BoxWorld* mWorld;
	Camera* mCamera;
	LevelManager* mLevel;
	LightSolver* mLightSolver;
};

/* TODO
 * Hantering av Star och Stardust
*/