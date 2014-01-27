#pragma once

/* An enum representing the controls in the game. It abstracts the actual input method and
 * allows that to be handled once in the game class, so that everything else only has to
 * care what action has been performed and not what triggered the action. */

namespace Controls
{
	enum Action {MOVE_LEFT, MOVE_RIGHT, JUMP, MENU, SIRIUS, INTERACT, UNUSED};
}