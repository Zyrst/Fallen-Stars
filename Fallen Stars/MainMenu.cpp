#include "MainMenu.h"

#include "Button.h"
#include "BaseResolution.h"
#include "Game.h"
#include "JumpingTest.h"

#include <iostream> // TODO Remove!

MainMenu::MainMenu(int id, ResourceCollection& resources):
	Menu(id)
{
	mBackground.setTexture(resources.getTexture("Assets/Menu/Main Menu.png"));

	sf::Vector2f width = sf::Vector2f((float)baseWidth, 0.0f);
	sf::Vector2f height = sf::Vector2f(0.0f, (float)baseHeight);

	sf::Texture& buttonTexture = resources.getTexture("Assets/Menu/Button.png");
	sf::Font& font = resources.getFont("Assets/Menu/24Janvier.otf");
	sf::Text start("Start", font, 30U);
	sf::Text settings("Settings", font, 30U);
	addButton(Button(START, width / 2.0f + height / 3.0f, buttonTexture, resources, start));
	addButton(Button(SETTINGS, width / 2.0f + height * 2.0f / 3.0f, buttonTexture, resources, settings));
}


MainMenu::~MainMenu(void)
{
}

void MainMenu::render(sf::RenderTarget& renderSurface)
{
	renderSurface.draw(mBackground);
	renderButtons(renderSurface);
}

void MainMenu::buttonPressed(int id)
{
	if(id == Buttons::START)
	{
		Game::instance()->loadNewState(new JumpingTest());
	}
	if(id == Buttons::SETTINGS)
	{
		std::cout << "Settings! Yay ^^ (There aren't any yet)";
	}
}