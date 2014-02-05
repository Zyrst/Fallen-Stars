#include "LogoState.h"

#include <cassert>
#include <cmath>

#include "Game.h"
#include "WaitState.h"

LogoState::LogoState():
	mTexture(),
	mSprite(),
	mTime()
{
	assert(mTexture.loadFromFile("../Debug/Logo.png"));
	mSprite.setTexture(mTexture);
}


LogoState::~LogoState(void)
{
}



void LogoState::load(){}

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

void LogoState::handleAction(Controls::Action, Controls::KeyState)
{

}