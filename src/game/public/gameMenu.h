#pragma once

#include "consoleUtil.h"
#include "character.h"
#include "constants.h"
#include <array>
#include <vector>

// render main menu before game combat and recover players characters selection and configuration
class gameMenu {
public:
	gameMenu();
	~gameMenu();

	// ask for each player to select and name their character. Then return selected character with data
	const std::array<character::data, constants::nbPlayers> gatherPlayerCharacterData();

protected:
	// fetch all files inside character's data folder and store their name as reference of available character to play
	void fectchAllCharactersLabel();

	// hydrate json data of a specific character
	const bool hydrateCharacterData(character::data& data, const std::string& characterLabel);

	// store character available to play
	std::vector<std::string> charactersLabel;
	consoleUtil console;
};