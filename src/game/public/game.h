#pragma once

#include "gameMenu.h"
#include "gameWorld.h"

class game final {
public:
	game();

protected:
	std::array<characterData, constants::nbPlayers> menu();
};