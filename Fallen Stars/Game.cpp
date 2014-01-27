#include "Game.h"
#include <SFML/Graphics.hpp>

Game::Game(void)
{
}


Game::~Game(void)
{
}

void Game::gameloop()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "My window");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

		window.clear(sf::Color::Black);

        // draw everything here...
        // window.draw(...);

        // end the current frame
        window.display();
    }

}