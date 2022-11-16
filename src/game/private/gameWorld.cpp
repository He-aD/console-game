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

const gameEndPlayerChoice gameWorld::start() {
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
	short healthNonEmptyIndex{ -1 }, i{ 0 };

	// check character health to calculate end condition
	for (auto& character : this->characters) {
		if (character->getCharacteristics().health.getAmount() != 0 && healthNonEmptyIndex == -1) {
			healthNonEmptyIndex = i;
		}
		else if (character->getCharacteristics().health.getAmount() != 0) { // more than one character is alive: no end condition
			return;
		}

		i++;
	}

	// calculate proper end condition
	if (healthNonEmptyIndex == -1) { // all characters are dead
		this->state = gameState::draw;
	}
	else {
		this->state = gameState(healthNonEmptyIndex);
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