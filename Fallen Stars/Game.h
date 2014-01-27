#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

class State;

class Game
{
public:
	Game();
	~Game();

	void gameloop();
	void setState(State* state);

private:
	sf::RenderWindow window;
	State* currentState;

	void handleEvent(sf::Event);
};

