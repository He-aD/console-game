#include "game.h"
#include "consoleUtil.h"

game::game() {
	consoleUtil::setSize();

	gameWorld world{ std::move(this->menu()) };

	world.start();
}

std::array<characterData, constants::nbPlayers> game::menu() {
	gameMenu menu;
	
	return menu.gatherPlayerCharacterData();
}