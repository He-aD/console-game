#pragma once

#include "gameMenu.h"
#include "gameWorld.h"

// master object that manage macro game phases: main menu, game combat, remake, back to menu or quit
class game final {
public:
	game();

protected:
	std::array<character::data, constants::nbPlayers> menu();
};