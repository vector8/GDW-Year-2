//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include "Game.h"

int main()
{
	flopse::Game* g = flopse::Game::getGame();

	g->run();

	return 0;
}