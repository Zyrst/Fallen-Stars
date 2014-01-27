#include "Game.h"

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <cassert>

#include "State.h"
#include "ControlMapping.h"



Game::Game():
	window()
{
	// TODO H�mta faktisk sk�rmuppl�sning + fixa fullsk�rm
	sf::VideoMode resolution = sf::VideoMode(1280, 720);
	window = sf::RenderWindow(resolution, "Fallen Stars");
	
	// TODO Skapa en f�rsta state
}


Game::~Game()
{
}

void Game::gameloop()
{
	sf::Clock clock;
    while (window.isOpen())
    {
		// Kolla alla kontroller och reagera p� input
        sf::Event event;
        while (window.pollEvent(event)) 
		{
			handleEvent(event);
		}

		// K�r all updateringskod
		float deltaTime = clock.getElapsedTime().asSeconds();
		clock.restart();

		currentState->update(deltaTime);

		// Rendera den nya framen
		window.clear(sf::Color::Black);
        currentState->render(window);

		window.display();
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
		window.close();
		return;
	}

	Controls::Action action = Controls::Action::UNUSED;
	
	if (event.type == sf::Event::KeyPressed) action = ControlMapping::getAction( event.key );
	else if (event.type == sf::Event::JoystickButtonPressed) action = ControlMapping::getAction( event.joystickButton );
	else if (event.type == sf::Event::JoystickMoved) action = ControlMapping::getAction( event.joystickMove );

	if( action != Controls::Action::UNUSED ) currentState->handleAction(action);
}