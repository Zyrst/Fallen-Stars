#include "Game.h"

#ifdef _WIN32
#ifdef _DEBUG
#include <Windows.h>
#endif
#endif

void main()
{
	Game game;
	game.run();

	#ifdef _WIN32
	#ifdef _DEBUG
		TerminateProcess(GetCurrentProcess(), EXIT_SUCCESS);
	#endif
	#endif
}