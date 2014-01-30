#include "Game.h"

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <cassert>

#include "State.h"
#include "ControlMapping.h"

int Width = 1280;
int Height = 720;

/* Temporarily stores the next state while waiting for the current state to end */
static State* nextState;

Game::Game()
{
	// TODO Get actual resolution + fix fullscreen
	sf::VideoMode resolution(Width, Height);
	window = new sf::RenderWindow(resolution, "Fallen Stars");
	
	// TODO Create a first state
}


Game::~Game()
{
	delete window;
	delete currentState;
}

void Game::run()
{
	assert(currentState != NULL);

	sf::Clock clock;
    while (window->isOpen())
    {
		// Kolla alla kontroller och reagera p� input
        sf::Event event;
        while (window->pollEvent(event)) 
		{
			handleEvent(event);
		}

		// K�r all updateringskod
		float deltaTime = clock.getElapsedTime().asSeconds();
		clock.restart();

		assert(currentState != NULL);
		currentState->update(deltaTime);

		// Rendera den nya framen
		window->clear(sf::Color::Black);

		assert(currentState != NULL);
        currentState->render(*window);

		window->display();

		// If we're going to swap state, do that at the end of the frame
		if(nextState != NULL) swapState();
    }
}

void Game::setState(State* state)
{
	assert(state != NULL);
	nextState = state;
}

void Game::swapState()
{
	assert(nextState != NULL);
	assert(currentState != NULL);

	delete currentState;
	currentState = nextState;
}

void Game::handleEvent(sf::Event event)
{
	assert(currentState != NULL);

	if (event.type == sf::Event::Closed)
	{
		window->close();
		return;
	}

	Controls::Action action = Controls::Action::UNUSED;
	Controls::KeyState keyState = Controls::KeyState::UNKNOWN;
	
	if (event.type == sf::Event::KeyPressed)
	{
		action = ControlMapping::getAction( event.key );
		keyState = Controls::KeyState::PRESSED;
	}
	else if (event.type == sf::Event::KeyReleased)
	{
		action = ControlMapping::getAction( event.key );
		keyState = Controls::KeyState::RELEASED;
	}
	
	// TODO Implement joystick
	// else if (event.type == sf::Event::JoystickButtonPressed) action = ControlMapping::getAction( event.joystickButton );
	// else if (event.type == sf::Event::JoystickMoved) action = ControlMapping::getAction( event.joystickMove );
	

	if( action != Controls::Action::UNUSED ) currentState->handleAction(action, keyState);
}