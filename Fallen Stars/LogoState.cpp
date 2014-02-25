#include "LogoState.h"

#include <cassert>
#include <cmath>

#include "Game.h"
#include "MainMenuState.h"
#include "PlatformState.h"

LogoState::LogoState():
	mSprite(),
	mTime()
{
}


LogoState::~LogoState(void)
{
}



void LogoState::load()
{
	std::string filename = "Assets/Menu/Logo.png";
	if(mTexture.loadFromFile(filename))
	{
		std::cout << "Texture " << filename << " loaded successfully" << std::endl;
	}
	mSprite.setTexture(mTexture);
}

void LogoState::update(const sf::Time& deltaTime)
{
	mTime += deltaTime;

	const float animationTime = 4; // seconds
	float sineWave = (float) sin(mTime.asSeconds() / animationTime * 3.141592);
	int brightness = (int) (255.0 * sineWave);
	if ( brightness < 0 ) brightness = 0;

	mSprite.setColor(sf::Color(brightness, brightness, brightness, 255));
	
	if(mTime.asSeconds() >= animationTime)
	{
		Game::instance()->loadNewState(new MainMenuState());
	}
}

void LogoState::render(sf::RenderWindow& window)
{
	window.draw(mSprite);
}

void LogoState::handleAction(Controls::Action action, Controls::KeyState)
{
	Game::instance()->loadNewState(new MainMenuState());
}