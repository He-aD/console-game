#include "game.h"
#include "consoleUtil.h"

game::game() {
	this->console.setSize();

	this->startNewGame(false);

	this->console.cursorCoordinate.Y = 2;
	this->console.renderTextXCentered("Thank you for playing. See you soon\n\n");
}

void game::startNewGame(const bool useStoredGameData) {
	gameEndAction action;

	if (!useStoredGameData) {
		this->charactersData = this->menu();
	}

	{ // force world out of scope
		gameWorld world{ this->charactersData };
		action = world.start();
	}

	if (action != gameEndAction::quit) {
		this->startNewGame(action == gameEndAction::remake);
	}
}

std::array<character::data, constants::nbPlayers> game::menu() {
	gameMenu menu;
	
	return menu.gatherPlayerCharacterData();
}