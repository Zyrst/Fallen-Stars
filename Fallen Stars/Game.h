#pragma once
#include <queue>

namespace sf{ class Event; }
namespace sf{ class RenderWindow; }
class State;

class RuntimeEvent
{
public:
	RuntimeEvent();
	virtual ~RuntimeEvent();

	virtual void run() = 0;
};

class Game
{
	public:
		Game();
		~Game();

		void run();
		void resize(int width, int height);
		void loadNewState(State* state); // Forwards to setState(), but first wraps the state in a LoadingState

		void postRuntimeEvent(RuntimeEvent* ev);

		static Game* instance();

	private:
		sf::RenderWindow *window;
		State* currentState;
		std::queue<RuntimeEvent*> runtimeEventQueue;

		void handleEvent(sf::Event&);
		void handleHeldKeys();
		void setState(State* state); // Sets a state to be replaced at the end of the current frame
		void swapState(); // Performs the swap of the upcoming and current states (at the end of the frame)
		void centerView();

		static Game* theGame;

		friend class LoadingState;
};

