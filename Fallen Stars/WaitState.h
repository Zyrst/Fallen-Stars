#pragma once
#include "state.h"
class WaitState : public State
{
public:
	void load() override;
	void update(sf::Time& deltaTime) override;
	void render(sf::RenderWindow& window) override;
	void handleAction(Controls::Action action, Controls::KeyState) override;
};

