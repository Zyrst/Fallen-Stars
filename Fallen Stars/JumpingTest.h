#pragma once
#include "State.h"
class JumpingTest :	public State
{
public:
	JumpingTest();
	virtual ~JumpingTest();

	virtual void update(sf::Time deltaTime) override;
	virtual void render(sf::RenderWindow& window) override;
	virtual void handleAction(Controls::Action action, Controls::KeyState) override;
};

