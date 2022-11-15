#pragma once

#include "gameMenu.h"
#include "gameWorld.h"

enum class gameEndAction {
	remake,
	newGame,
	quit
};

// master object that only manage macro game phases: main menu, game combat, remake, back to menu or quit
class game final {
public:
	game();

protected:
	// start a new game if bool param is false display menu first to gather player's configuration
	void startNewGame(const bool useStoredGameData);

	std::array<character::data, constants::nbPlayers> menu();

protected:
	// keep a copy of game data for remake
	std::array<character::data, constants::nbPlayers> charactersData;

	consoleUtil console;
};