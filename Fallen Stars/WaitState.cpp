#include "WaitState.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <chrono>
#include <thread>

void WaitState::load()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
}

void WaitState::update(const sf::Time& deltaTime)
{

}

void WaitState::render(sf::RenderWindow& window)
{
	window.clear(sf::Color(255,255,255,255));
}

void WaitState::handleAction(Controls::Action action, Controls::KeyState)
{

}

std::string WaitState::getTypeName()
{
	return "Wait State";
}