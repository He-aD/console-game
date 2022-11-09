#include "gameWorld.h"
#include <utility>

gameWorld gameWorld::make(const std::array<characterData, constants::nbPlayers> datas) {
	sharedCharacter characters[constants::nbPlayers];

	for (unsigned short i = 0; i < constants::nbPlayers; i++) {
		characters[i] = std::make_shared<character>(
			character::make(datas[i]));
	}

	gameRenderer renderer(characters);

	return gameWorld(std::move(renderer), std::move(characters));
}

gameWorld::gameWorld(gameRenderer inRenderer, sharedCharacter inCharacters[constants::nbPlayers])
	: renderer(inRenderer) {
	for (unsigned short i = 0; i < constants::nbPlayers; i++) {
		this->characters[i] = inCharacters[i];
	}
}