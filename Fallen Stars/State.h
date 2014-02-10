#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "Controls.h"
#include "ResourceCollection.h"

/* States represent the various different sections of the game. The state contatins the environment that the
 * game currently uses. The active state also takes care of all the rendering performed during the current
 * frame. When a state gets replaced, it is destroyed. 
 *
 * Resource management is handled in two ways, both globally and locally. The local resources are stored inside 
 * the state and share their life length. */
class State
{
	public:
		virtual void load() {}; // Load local resources and load or link in global resources.
		virtual void update(sf::Time deltaTime) = 0;
		virtual void render(sf::RenderWindow& window) = 0;
		virtual void handleAction(Controls::Action action, Controls::KeyState) = 0;

	protected:
		ResourceCollection mResourceCollection;
};