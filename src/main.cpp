#include "Game.h"

int main()
{
#ifdef _DEBUG
	ShowWindow(GetConsoleWindow(), SW_SHOW);
#else
	ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif	
	Game game;
	if (game.Construct(480, 320, 2, 2))
		game.Start();
	return 0;
}