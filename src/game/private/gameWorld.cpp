#include "gameWorld.h"
#include <utility>

gameWorld gameWorld::make(const std::array<characterData, constants::nbPlayers> datas) {
	sharedCharacter characters[constants::nbPlayers];

	for (unsigned short i = 0; i < constants::nbPlayers; i++) {
		characters[i] = std::move(
			character::make(datas[i]));
	}

	gameRenderer renderer(characters);

	return gameWorld(std::move(renderer), std::move(characters));
}

gameWorld::gameWorld(gameRenderer inRenderer, sharedCharacter inCharacters[constants::nbPlayers])
	: renderer(inRenderer) {
	this->state = gameState::waitToStart;
	this->turn = 1;

	for (unsigned short i = 0; i < constants::nbPlayers; i++) {
		this->characters[i] = inCharacters[i];
	}
}

const gameWorld::gameState gameWorld::start() {
	this->renderer.askPlayerReady();

	this->state = gameState::inProgress;
	do {
		this->renderer.render(*this);

		for (unsigned short i = 0; i < constants::nbPlayers; i++) {
			this->renderer.doPlayerUseAbility(i);
		}

		this->characters[0]->characteristics.health.takeDamage(this->characters[1]->characteristics.attackPower);
		this->characters[1]->characteristics.health.takeDamage(this->characters[0]->characteristics.attackPower);

		this->testGameEnd();

		this->turn++;
	} while (this->state == gameState::inProgress);

	return this->state;
}

void gameWorld::testGameEnd() {
	if (this->characters[0]->characteristics.health.getAmount() == 0 && 
		this->characters[1]->characteristics.health.getAmount() == 0) {
		this->state = gameState::draw;
	}
	else if (this->characters[0]->characteristics.health.getAmount() == 0) {
		this->state = gameState::player2Win;
	}
	else if (this->characters[1]->characteristics.health.getAmount() == 0) {
		this->state = gameState::player1Win;
	}
}