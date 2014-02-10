#include "LogoState.h"

#include <cassert>
#include <cmath>

#include "Game.h"
#include "WaitState.h"

LogoState::LogoState():
	mSprite(),
	mTime()
{
	mSprite.setTexture(mResourceCollection.getTexture("../Debug/Logo.png"));
}


LogoState::~LogoState(void)
{
}



void LogoState::load()
{
	mResourceCollection.preloadTexture("../Debug/Logo.png");
}

void LogoState::update(sf::Time deltaTime)
{
	mTime += deltaTime;

	const float animationTime = 4; // seconds
	float sineWave = (float) sin(mTime.asSeconds() / animationTime * 3.141592);
	int brightness = (int) (255.0 * sineWave);
	if ( brightness < 0 ) brightness = 0;

	mSprite.setColor(sf::Color(brightness, brightness, brightness, 255));
	
	if(mTime.asSeconds() >= animationTime)
	{
		Game::instance()->loadNewState(new WaitState());
	}
}

void LogoState::render(sf::RenderWindow& window)
{
	window.draw(mSprite);
}

void LogoState::handleAction(Controls::Action action, Controls::KeyState)
{
	Game::instance()->loadNewState(new WaitState());
}