#include "gameWorld.h"
#include <utility>
#include "game.h"

gameWorld::gameWorld(const std::array<character::data, constants::nbPlayers> datas) {
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

const gameEndAction gameWorld::start() {
	this->renderer->askPlayerReady();
	this->state = gameState::inProgress;

	do {
		// new turn 
		this->turn++;
		this->newTurnDelegate.broadcast(this->turn);

		// render new turn
		this->renderer->renderNewTurn(*this);

		// if inputed by players execute their special abilities
		for (unsigned short i = 0; i < constants::nbPlayers; i++) {
			this->combatResults[i].reset();

			if (this->characters[i]->getAbility().getNbTurnToBeAvailable() == 0 && this->renderer->doPlayerUseAbility(i)) {
				this->combatResults[i].playerInputAbility = true;
				this->combatResults[i].abilitySucceeded = this->characters[i]->getAbility().process(
					this->characters[(i + 1) % constants::nbPlayers]->getCharacteristics());
			}
		}

		// both characters try to make damage to each other
		for (unsigned short i = 0; i < constants::nbPlayers; i++) {
			this->combatResults[i].attackSucceeded = this->master->tryMakeDamage(
				this->characters[i]->getCharacteristics(),
				this->characters[(i + 1) % constants::nbPlayers]->getCharacteristics());
		}

		// test end game condition
		this->testGameEnd();

		// render combat result
		this->renderer->renderCombatResult(*this);

	} while (this->state == gameState::inProgress);

	// render end screen and ask for: remake, back to menu or quit
	return this->renderer->renderEndScreen(*this);
}

void gameWorld::testGameEnd() {
	// check end conditions
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
}

const gameWorld::combatResult& gameWorld::getCombatResults(const unsigned short index) const {
	return this->combatResults[index % constants::nbPlayers]; 
}
//////////////////////// DATA ////////////////////////////////////

void gameWorld::combatResult::reset() {
	this->abilitySucceeded = false;
	this->attackSucceeded = false;
	this->playerInputAbility = false;
}