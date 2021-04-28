#include <curses.h>
#include <stdlib.h>
#include <time.h>
#include "Snake.h"

int main(void) {

	Game* game = Game().loadGameProgress();
	if(game == NULL)
		game = new Game(50,50);
	else{
		game->setup();
	}

	game->execute();
	endwin();
}