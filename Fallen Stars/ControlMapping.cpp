#include "ControlMapping.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Joystick.hpp>

Controls::Action ControlMapping::getAction(sf::Event::KeyEvent key)
{
	switch(key.code)
	{
		case sf::Keyboard::Key::A:
			return Controls::MOVE_LEFT;
		case sf::Keyboard::Key::D:
			return Controls::MOVE_RIGHT;
		case sf::Keyboard::Key::Space:
			return Controls::JUMP;
		case sf::Keyboard::Key::E:
			return Controls::INTERACT;
		case sf::Keyboard::Key::Escape:
			return Controls::MENU;
		case sf::Keyboard::Key::H:
			return Controls::SIRIUS;
		default:
			return Controls::UNUSED;
	}
}

Controls::Action ControlMapping::getAction(sf::Event::JoystickButtonEvent joystickButton)
{
	// TODO Implement joystick button mapping
	return Controls::Action::UNUSED;
}

Controls::Action ControlMapping::getAction(sf::Event::JoystickMoveEvent joystickMove)
{
	// TODO Implement joystick movement mapping
	return Controls::Action::UNUSED;
}