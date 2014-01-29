#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "Controls.h"

/* States represent the various different sections of the game. The state contatins the environment that the
 * game currently uses. The active state also takes care of all the rendering performed during the current
 * frame. When a state gets replaced, it is destroyed. */
class State
{
	public:
		virtual ~State();
		virtual void update(float deltaTime) = 0;
		virtual void render(sf::RenderWindow& window) = 0;
		virtual void handleAction(Controls::Action action, Controls::KeyState) = 0;
};

