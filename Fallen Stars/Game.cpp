#include "Game.h"

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <cassert>

#include "State.h"
#include "ControlMapping.h"

int Width = 1280;
int Height = 720;

Game::Game()
{
	// TODO Hämta faktisk skärmupplösning + fixa fullskärm
	sf::VideoMode resolution(Width, Height);
	window = new sf::RenderWindow(resolution, "Fallen Stars");
	
	// TODO Skapa en första state
}


Game::~Game()
{
	delete window;
}

void Game::run()
{
	assert(currentState != NULL);

	sf::Clock clock;
    while (window->isOpen())
    {
		// Kolla alla kontroller och reagera på input
        sf::Event event;
        while (window->pollEvent(event)) 
		{
			handleEvent(event);
		}

		// Kör all updateringskod
		float deltaTime = clock.getElapsedTime().asSeconds();
		clock.restart();

		assert(currentState != NULL);
		currentState->update(deltaTime);

		// Rendera den nya framen
		window->clear(sf::Color::Black);

		assert(currentState != NULL);
        currentState->render(*window);

		window->display();
    }
}

void Game::setState(State* state)
{
	assert(state != NULL);
	assert(currentState != NULL);

	delete currentState;
	currentState = state;
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
	
	if (event.type == sf::Event::KeyPressed) action = ControlMapping::getAction( event.key );
	else if (event.type == sf::Event::JoystickButtonPressed) action = ControlMapping::getAction( event.joystickButton );
	else if (event.type == sf::Event::JoystickMoved) action = ControlMapping::getAction( event.joystickMove );

	if( action != Controls::Action::UNUSED ) currentState->handleAction(action);
}