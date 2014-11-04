#include "Game.h"

int main()
{
	Game* g = Game::getGame();

	g->run();

	return 0;
}