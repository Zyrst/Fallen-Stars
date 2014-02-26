#pragma once

/* An enum representing the controls in the game. It abstracts the actual input method and
 * allows that to be handled once in the game class, so that everything else only has to
 * care what action has been performed and not what triggered the action. */

namespace Controls
{
	enum Action {UP, DOWN, LEFT, RIGHT, JUMP, MENU, CONFIRM, SIRIUS, INTERACT, UNUSED}; // UNUSED is also used as iteration end value
	enum KeyState {PRESSED, RELEASED, HELD};
	
	Action& operator++(Action& value);
}