#include "gameWorld.h"
#include <utility>

gameWorld::gameWorld(const std::array<characterData, constants::nbPlayers> datas) {
	this->state = gameState::waitToStart;
	this->turn = 0;

	// instanciate game master
	this->master = std::make_unique<gameMaster>();

	// instanciate characters
	for (unsigned short i = 0; i < constants::nbPlayers; i++) {
		this->characters[i] = character::make(datas[i], *this);
	}

	// instanciate renderer
	this->renderer = std::make_unique<gameRenderer>(this->characters);
}

const gameWorld::gameState gameWorld::start() {
	this->renderer->askPlayerReady();
	this->state = gameState::inProgress;

	do {
		// new turn 
		this->turn++;
		this->newTurnDelegate.broadcast(this->turn);

		// render new turn
		this->renderer->render(*this);

		// if inputed by players execute their special abilities
		for (unsigned short i = 0; i < constants::nbPlayers; i++) {
			if (this->renderer->doPlayerUseAbility(i)) {
				this->characters[i]->getAbility().process(
					this->characters[(i + 1) % constants::nbPlayers]->getCharacteristics());
			}
		}

		// both characters try to make damage to each other
		for (unsigned short i = 0; i < constants::nbPlayers; i++) {
			this->master->tryMakeDamage(this->characters[i]->getCharacteristics(),
				this->characters[(i + 1) % constants::nbPlayers]->getCharacteristics());
		}

		// test end game condition
		this->testGameEnd();
	} while (this->state == gameState::inProgress);

	return this->state;
}

void gameWorld::testGameEnd() {
	if (this->characters[0]->getCharacteristics().health.getAmount() == 0 &&
		this->characters[1]->getCharacteristics().health.getAmount() == 0) {
		this->state = gameState::draw;
	}
	else if (this->characters[0]->getCharacteristics().health.getAmount() == 0) {
		this->state = gameState::player2Win;
	}
	else if (this->characters[1]->getCharacteristics().health.getAmount() == 0) {
		this->state = gameState::player1Win;
	}

	// render end screen and ask for remake, back to menu or quit
	if (this->state != gameState::inProgress) {
		
	}
}