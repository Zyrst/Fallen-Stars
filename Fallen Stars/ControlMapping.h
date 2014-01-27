#pragma once

#include <SFML/Window/Event.hpp>

#include "Controls.h"

namespace ControlMapping 
{
	Controls::Action getAction(sf::Event::KeyEvent);
	Controls::Action getAction(sf::Event::JoystickButtonEvent);
	Controls::Action getAction(sf::Event::JoystickMoveEvent);
};