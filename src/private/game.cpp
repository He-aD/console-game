#include "game.h"

game::game() {
	gameWorld g = gameWorld::make(std::move(this->menu()));

	g.start();
}

std::array<characterData, constants::nbPlayers> game::menu() {
	gameMenu menu;
	
	return menu.gatherPlayerCharacterData();
}