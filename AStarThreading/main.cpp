#include <iostream>

#include "Game.h"


int main(int argc, char ** argv) {
	Game game;

	std::cout << "Initialising Game" << std::endl;

	if (!game.init()) {
		std::cout << "Failed to init game" << '\n';
	}

	//run the game loop
	game.loop();

	game.destroy();

	return 0;
}