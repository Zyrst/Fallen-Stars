#include "MainMenu.h"

#include "Button.h"
#include "BaseResolution.h"
#include "Game.h"
#include "JumpingTest.h"

MainMenu::MainMenu(ResourceCollection& resources)
{
	mBackground.setTexture(resources.getTexture("../Debug/Main Menu.png"));

	sf::Vector2f resolution = sf::Vector2f((float)baseWidth, (float)baseHeight);

	sf::Texture& buttonTexture = resources.getTexture("../Debug/Button.png");
	sf::Font& font = resources.getFont("../Debug/24Janvier.otf");
	sf::Text start("Start", font, 30U);
	addButton(Button(START, resolution / 2.0f, buttonTexture, start));
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
}