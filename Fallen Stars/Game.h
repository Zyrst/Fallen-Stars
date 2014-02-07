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
		void resize(sf::VideoMode);
		void loadNewState(State* state); // Forwards to setState(), but first wraps the state in a LoadingState

		static Game* instance();

	private:
		sf::RenderWindow *window;
		State* currentState;

		void handleEvent(sf::Event);
		void handleHeldKeys();
		void setState(State* state); // Sets a state to be replaced at the end of the current frame
		void swapState(); // Performs the swap of the upcoming and current states (at the end of the frame)

		static Game* theGame;

		friend class LoadingState;
};

