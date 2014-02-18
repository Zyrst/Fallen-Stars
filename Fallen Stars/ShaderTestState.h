#pragma once
#include "State.h"
class ShaderTestState :
	public State
{
public:
	ShaderTestState();
	virtual ~ShaderTestState();

	virtual void update(const sf::Time& deltaTime) override;
	virtual void render(sf::RenderWindow& window) override;
	virtual void handleAction(Controls::Action, Controls::KeyState) override;
};

