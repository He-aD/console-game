#pragma once

#include "consoleUtil.h"
#include "character.h"
#include "constants.h"
#include <array>

class gameMenu {
public:
	gameMenu();
	~gameMenu();

	const std::array<characterData, constants::nbPlayers> gatherCharacterDatas();

protected:
	consoleUtil console;
};