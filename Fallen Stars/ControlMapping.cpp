#include "ControlMapping.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Joystick.hpp>

Controls::Action ControlMapping::getAction(sf::Event::KeyEvent key)
{
	switch(key.code)
	{
		case sf::Keyboard::Key::A:
			return Controls::Action::MOVE_LEFT;
		case sf::Keyboard::Key::D:
			return Controls::Action::MOVE_RIGHT;
		case sf::Keyboard::Key::Space:
			return Controls::Action::JUMP;
		case sf::Keyboard::Key::E:
			return Controls::Action::INTERACT;
		case sf::Keyboard::Key::Escape:
			return Controls::Action::MENU;
		case sf::Keyboard::Key::H:
			return Controls::Action::SIRIUS;
		default:
			return Controls::Action::UNUSED;
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