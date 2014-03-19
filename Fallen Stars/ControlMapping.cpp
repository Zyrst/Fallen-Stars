#include "ControlMapping.h"

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Joystick.hpp>

namespace Keys
{
	enum KeyMap
	{
		UP		 = sf::Keyboard::Key::W,
		DOWN	 = sf::Keyboard::Key::S, 
		LEFT	 = sf::Keyboard::Key::A,
		RIGHT	 = sf::Keyboard::Key::D,
		JUMP	 = sf::Keyboard::Key::Space,
		INTERACT = sf::Keyboard::Key::E,
		MENU	 = sf::Keyboard::Key::Escape,
		CONFIRM	 = sf::Keyboard::Key::Return,
		SIRIUS	 = sf::Keyboard::Key::H,
		CHEAT1	 = sf::Keyboard::Key::F5,
		CHEAT2	 = sf::Keyboard::Key::F6,
		CHEAT3	 = sf::Keyboard::Key::F7,
		DEBUG	 = sf::Keyboard::Key::F8
	};
}

namespace AlternativeKeys
{
	enum KeyMap
	{
		UP	  = sf::Keyboard::Key::Up,
		DOWN  = sf::Keyboard::Key::Down,
		LEFT  = sf::Keyboard::Key::Left,
		RIGHT = sf::Keyboard::Key::Right,
		MENU  = sf::Keyboard::Key::P
	};
}

Controls::Action ControlMapping::getAction(sf::Event::KeyEvent key)
{
	switch(key.code)
	{
		case Keys::KeyMap::UP:		 return Controls::Action::UP;
		case Keys::KeyMap::DOWN:	 return Controls::Action::DOWN;
		case Keys::KeyMap::LEFT:	 return Controls::Action::LEFT;
		case Keys::KeyMap::RIGHT:	 return Controls::Action::RIGHT;
		case Keys::KeyMap::JUMP:	 return Controls::Action::JUMP;
		case Keys::KeyMap::INTERACT: return Controls::Action::INTERACT;
		case Keys::KeyMap::MENU:	 return Controls::Action::MENU;
		case Keys::KeyMap::CONFIRM:  return Controls::Action::CONFIRM;
		case Keys::KeyMap::SIRIUS:	 return Controls::Action::SIRIUS;
		case Keys::KeyMap::CHEAT1:	 return Controls::Action::CHEAT1;
		case Keys::KeyMap::CHEAT2:	 return Controls::Action::CHEAT2;
		case Keys::KeyMap::CHEAT3:	 return Controls::Action::CHEAT3;
		case Keys::KeyMap::DEBUG:	 return Controls::Action::DEBUG;

		case AlternativeKeys::KeyMap::UP:	 return Controls::Action::UP;
		case AlternativeKeys::KeyMap::DOWN:  return Controls::Action::DOWN;
		case AlternativeKeys::KeyMap::LEFT:	 return Controls::Action::LEFT;
		case AlternativeKeys::KeyMap::RIGHT: return Controls::Action::RIGHT;
		case AlternativeKeys::KeyMap::MENU:	 return Controls::Action::MENU;

		default: return Controls::Action::UNUSED;
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
		case Controls::Action::UP:		 return static_cast <sf::Keyboard::Key> ( Keys::KeyMap::UP );
		case Controls::Action::DOWN:	 return static_cast <sf::Keyboard::Key> ( Keys::KeyMap::DOWN );
		case Controls::Action::LEFT:	 return static_cast <sf::Keyboard::Key> ( Keys::KeyMap::LEFT );
		case Controls::Action::RIGHT:	 return static_cast <sf::Keyboard::Key> ( Keys::KeyMap::RIGHT );
		case Controls::Action::JUMP:	 return static_cast <sf::Keyboard::Key> ( Keys::KeyMap::JUMP );
		case Controls::Action::INTERACT: return static_cast <sf::Keyboard::Key> ( Keys::KeyMap::INTERACT );
		case Controls::Action::MENU:	 return static_cast <sf::Keyboard::Key> ( Keys::KeyMap::MENU );
		case Controls::Action::CONFIRM:  return static_cast <sf::Keyboard::Key> ( Keys::KeyMap::CONFIRM );
		case Controls::Action::SIRIUS:	 return static_cast <sf::Keyboard::Key> ( Keys::KeyMap::SIRIUS );
		case Controls::Action::CHEAT1:	 return static_cast <sf::Keyboard::Key> ( Keys::KeyMap::CHEAT1 );
		case Controls::Action::CHEAT2:	 return static_cast <sf::Keyboard::Key> ( Keys::KeyMap::CHEAT2 );
		case Controls::Action::CHEAT3:	 return static_cast <sf::Keyboard::Key> ( Keys::KeyMap::CHEAT3 );
		case Controls::Action::DEBUG:	 return static_cast <sf::Keyboard::Key> ( Keys::KeyMap::DEBUG );
		
		default: return sf::Keyboard::Key::Unknown;
	}
}

sf::Keyboard::Key ControlMapping::getAlternativeKey(Controls::Action action)
{
	switch(action)
	{
		case Controls::Action::UP:	  return static_cast <sf::Keyboard::Key> ( AlternativeKeys::KeyMap::UP );
		case Controls::Action::DOWN:  return static_cast <sf::Keyboard::Key> ( AlternativeKeys::KeyMap::DOWN );
		case Controls::Action::LEFT:  return static_cast <sf::Keyboard::Key> ( AlternativeKeys::KeyMap::LEFT );
		case Controls::Action::RIGHT: return static_cast <sf::Keyboard::Key> ( AlternativeKeys::KeyMap::RIGHT );
		case Controls::Action::MENU:  return static_cast <sf::Keyboard::Key> ( AlternativeKeys::KeyMap::MENU );
		
		default: return sf::Keyboard::Key::Unknown;
	}
}