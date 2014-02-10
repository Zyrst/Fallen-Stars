#include "ControlMapping.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Joystick.hpp>


Controls::Action ControlMapping::getAction(sf::Event::KeyEvent key)
{
	switch(key.code)
	{
		case sf::Keyboard::Key::W:
			return Controls::Action::UP;
		case sf::Keyboard::Key::S:
			return Controls::Action::DOWN;
		case sf::Keyboard::Key::A:
			return Controls::Action::LEFT;
		case sf::Keyboard::Key::D:
			return Controls::Action::RIGHT;
		case sf::Keyboard::Key::Space:
			return Controls::Action::JUMP;
		case sf::Keyboard::Key::E:
			return Controls::Action::INTERACT;
		case sf::Keyboard::Key::Escape:
			return Controls::Action::MENU;
		case sf::Keyboard::Key::Return:
			return Controls::Action::CONFIRM;
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

sf::Keyboard::Key ControlMapping::getKey(Controls::Action action)
{
	switch(action)
	{
		case Controls::Action::UP:
			return sf::Keyboard::Key::W;
		case Controls::Action::DOWN:
			return sf::Keyboard::Key::S;
		case Controls::Action::LEFT:
			return sf::Keyboard::Key::A;
		case Controls::Action::RIGHT:
			return sf::Keyboard::Key::D;
		case Controls::Action::JUMP:
			return sf::Keyboard::Key::Space;
		case Controls::Action::INTERACT:
			return sf::Keyboard::Key::E;
		case Controls::Action::MENU:
			return sf::Keyboard::Key::Escape;
		case Controls::Action::CONFIRM:
			return sf::Keyboard::Key::Return;
		case Controls::Action::SIRIUS:
			return sf::Keyboard::Key::H;
		default:
			sf::Keyboard::Key::Unknown;
	}
}