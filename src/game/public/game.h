#pragma once

#include "gameMenu.h"
#include "gameWorld.h"

// represent player choice made at the end of a combat
enum class gameEndPlayerChoice { 
	remake,
	newGame,
	quit
};

// master object that only manage macro game phases: main menu, game combat, remake, back to menu or quit
class game final {
public:
	game();

protected:
	// start a new game. If bool param is false display menu first to gather player's configuration
	void startNewGame(const bool useStoredGameData);

	// display menu and return gathered player's configuration
	std::array<character::data, constants::nbPlayers> menu();

protected:
	// keep a copy of game data for eventual remake
	std::array<character::data, constants::nbPlayers> charactersData;

	consoleUtil console;
};