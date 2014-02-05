#pragma once
#include "ControlMapping.h"
#include "Entity.h"
#include "Player.h"
#include <vector>
#include "State.h"

class PlatformState: public State
{
public:
	PlatformState();
	~PlatformState();
	void update(sf::Time deltaTime);
	void render(sf::RenderWindow& window);
	void handleAction(Controls::Action action, Controls::KeyState keystate);
	void clear();


private:
	std::vector <class Entity&> mEntityVector;

};

