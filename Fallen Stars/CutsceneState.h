#pragma once
#include "State.h"

#include <string>
#include <sfeMovie/Movie.hpp>

class CutsceneState : public State
{
public:
	CutsceneState(std::string filename, State* state);
	void load() override; // Load resources and set up the state
	void update(const sf::Time& deltaTime) override;
	void render(sf::RenderWindow& window) override;
	void handleAction(Controls::Action, Controls::KeyState) override;

private:
	void updateSize(sf::RenderWindow& window);

	std::string filename;
	sfe::Movie video;
	bool loadFailed;

	State* nextState;
};
