#include "game.h"

game::game() {
	gameWorld world{ std::move(this->menu()) };

	world.start();
}

std::array<characterData, constants::nbPlayers> game::menu() {
	gameMenu menu;
	
	return menu.gatherPlayerCharacterData();
}