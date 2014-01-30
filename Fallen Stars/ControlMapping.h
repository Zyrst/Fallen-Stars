#pragma once

#include <SFML/Window/Event.hpp>

#include "Controls.h"

class ControlMapping 
{
public:
	static Controls::Action getAction(sf::Event::KeyEvent);
	static Controls::Action getAction(sf::Event::JoystickButtonEvent);
	static Controls::Action getAction(sf::Event::JoystickMoveEvent);

	static sf::Keyboard::Key getKey(Controls::Action);
};