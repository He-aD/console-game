#include "game.h"
#include "consoleUtil.h"

game::game() {
	this->console.setSize();

	this->startNewGame(false);

	this->console.cursorCoordinate.Y = 2;
	this->console.renderTextXCentered("Thank you for playing. See you soon\n\n");
}

void game::startNewGame(const bool useStoredGameData) {
	gameEndPlayerChoice choice;

	if (!useStoredGameData) {
		this->charactersData = this->menu();
	}

	{ // force world out of scope
		gameWorld world{ this->charactersData };
		choice = world.start();
	}

	if (choice != gameEndPlayerChoice::quit) {
		this->startNewGame(choice == gameEndPlayerChoice::remake);
	}
}

std::array<character::data, constants::nbPlayers> game::menu() {
	gameMenu menu;
	
	return menu.gatherPlayerCharacterData();
}