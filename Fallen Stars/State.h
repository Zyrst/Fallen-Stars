#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "Controls.h"

class State
{
public:
	virtual ~State(void);
	virtual void update(float deltaTime) = 0;
	virtual void render(sf::RenderWindow& window) = 0;
	virtual void handleAction(Controls::Action action, Controls::KeyState) = 0;
};

