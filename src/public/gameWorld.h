#pragma once

#include "gameRenderer.h"
#include <memory>
#include "constants.h"
#include <array>

class gameWorld {
public:
	static gameWorld make(const std::array<characterData, constants::nbPlayers> datas);

	gameWorld(gameRenderer inRenderer, sharedCharacter inCharacters[constants::nbPlayers]);

private:
	sharedCharacter characters[constants::nbPlayers];
	gameRenderer renderer;
};