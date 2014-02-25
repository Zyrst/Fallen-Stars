#pragma once

#include <SFML/Window/Event.hpp>

namespace sf{class RenderWindow;}
class State;

class Game
{
	public:
		Game();
		~Game();

		void run();
		void resize(int width, int height);
		void loadNewState(State* state); // Forwards to setState(), but first wraps the state in a LoadingState

		static Game* instance();

	private:
		sf::RenderWindow *window;
		State* currentState;

		void handleEvent(sf::Event);
		void handleHeldKeys();
		void setState(State* state); // Sets a state to be replaced at the end of the current frame
		void swapState(); // Performs the swap of the upcoming and current states (at the end of the frame)
		void centerView();

		static Game* theGame;

		friend class LoadingState;
};

