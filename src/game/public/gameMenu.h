#pragma once

#include "consoleUtil.h"
#include "character.h"
#include "constants.h"
#include <array>
#include <vector>

class gameMenu {
public:
	gameMenu();
	~gameMenu();

	// ask for each player to select and name their character. Then return selected charcarter with data
	const std::array<characterData, constants::nbPlayers> gatherPlayerCharacterData();

protected:
	// fetch all characters inside characterDatas folder and store their name as reference of available character to play
	void fectchAllCharactersLabel();
	// hydrate json data of a specific character
	const bool hydrateCharacterData(characterData& data, const std::string& characterLabel);

	std::vector<std::string> charactersLabel;
	consoleUtil console;
};