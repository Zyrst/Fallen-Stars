#include "Game.h"

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <cassert>

#include "State.h"
#include "ControlMapping.h"
#include "LoadingState.h"
#include "LogoState.h"

int Width = 1920;
int Height = 1080;

/* Temporarily stores the next state while waiting for the current state to end */
static State* nextState;

Game* Game::theGame = NULL;

Game::Game()
{
	// TODO Get actual resolution + fix fullscreen
	sf::VideoMode resolution(Width, Height);
	
	window = new sf::RenderWindow(resolution, "Fallen Stars");
	
	// TODO Set viewport to 1080 to fix rendering scale for other monitor sizes

	// TODO Create a first state
	currentState = new LogoState();

	Game::theGame = this;
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
		// Check all controls and react to input
		sf::Event event;
        while (window->pollEvent(event)) 
		{
			handleEvent(event);
		}
		handleHeldKeys();

		// Do all the updates
		sf::Time deltaTime = clock.getElapsedTime();
		clock.restart();

		assert(currentState != NULL);
		currentState->update(deltaTime);

		// Render the frame
		window->clear(sf::Color::Black);

		assert(currentState != NULL);
        currentState->render(*window);

		window->display();

		// If we're going to swap state, do that at the end of the frame
		if(nextState != NULL) swapState();
    }

}

void Game::loadNewState(State* state)
{
	assert(state != NULL);
	assert(nextState == NULL);
	LoadingState* loadingState = new LoadingState(state);
	loadingState->load(); // This only takes time once. Every call after that will simply retrieves a reference
	nextState = loadingState;
}

void Game::setState(State* state)
{
	assert(state != NULL);
	assert(nextState == NULL);
	nextState = state;
}

void Game::swapState()
{
	assert(nextState != NULL);
	assert(currentState != NULL);
	assert(currentState != nextState);

	delete currentState;
	currentState = nextState;
	nextState = NULL;
}

Game* Game::instance()
{
	return theGame;
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
	Controls::KeyState keyState;
	
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

void Game::handleHeldKeys()
{
	if( sf::Keyboard::isKeyPressed( ControlMapping::getKey( Controls::Action::UP ) ) ) currentState->handleAction( Controls::Action::UP, Controls::KeyState::HELD );
	if( sf::Keyboard::isKeyPressed( ControlMapping::getKey( Controls::Action::DOWN ) ) ) currentState->handleAction( Controls::Action::DOWN, Controls::KeyState::HELD );
	if( sf::Keyboard::isKeyPressed( ControlMapping::getKey( Controls::Action::LEFT ) ) ) currentState->handleAction( Controls::Action::LEFT, Controls::KeyState::HELD );
	if( sf::Keyboard::isKeyPressed( ControlMapping::getKey( Controls::Action::RIGHT ) ) ) currentState->handleAction( Controls::Action::RIGHT, Controls::KeyState::HELD );
	if( sf::Keyboard::isKeyPressed( ControlMapping::getKey( Controls::Action::JUMP ) ) ) currentState->handleAction( Controls::Action::JUMP, Controls::KeyState::HELD );
	if( sf::Keyboard::isKeyPressed( ControlMapping::getKey( Controls::Action::INTERACT ) ) ) currentState->handleAction( Controls::Action::INTERACT, Controls::KeyState::HELD );
	if( sf::Keyboard::isKeyPressed( ControlMapping::getKey( Controls::Action::MENU ) )) currentState->handleAction( Controls::Action::MENU, Controls::KeyState::HELD );
	if( sf::Keyboard::isKeyPressed( ControlMapping::getKey( Controls::Action::SIRIUS ) ) ) currentState->handleAction( Controls::Action::SIRIUS, Controls::KeyState::HELD );
}