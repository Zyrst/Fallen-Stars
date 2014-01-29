#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include "State.h"

class Game
{
	public:
		Game();
		~Game();

		void run();
		void setState(State* state);

	private:
		sf::RenderWindow *window;
		State* currentState;

		void handleEvent(sf::Event);
};

