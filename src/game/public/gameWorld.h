#pragma once

#include "gameRenderer.h"
#include <memory>
#include "constants.h"
#include <array>

class gameWorld final {
public:
	enum class gameState {
		player1Win,
		player2Win,
		draw,
		waitToStart,
		inProgress,
	};

	static gameWorld make(const std::array<characterData, constants::nbPlayers> datas);

	gameWorld(gameRenderer inRenderer, sharedCharacter inCharacters[constants::nbPlayers]);

	// start the game and return the result
	const gameState start();

public:
	const gameState getState() const { return this->state; }
	const unsigned short getTurn() const { return this->turn; }

private:
	// test if of the game end conditions is true
	void testGameEnd();

	unsigned short turn;
	gameState state;
	sharedCharacter characters[constants::nbPlayers];
	gameRenderer renderer;
};