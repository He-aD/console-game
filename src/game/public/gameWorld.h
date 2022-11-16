#pragma once

#include "gameRenderer.h"
#include <memory>
#include "constants.h"
#include <array>
#include "delegate.h"
#include "gameMaster.h"

enum class gameEndPlayerChoice;

// manage game loop and instanciates game classes (renderer, characters etc.)
class gameWorld final {
public:
	enum class gameState { // represent current game state
		player1Win,
		player2Win,
		draw,
		waitToStart,
		inProgress,
	};

	// store how players and characters behaved each turn
	// used to display combat text by renderer
	struct combatResult { 
		combatResult();

		void reset();

		bool playerInputAbility;
		bool abilitySucceeded;
		unsigned short damageMade;
	};

public:
	// instaciates game master, renderer and characters
	gameWorld(const std::array<character::data, constants::nbPlayers> datas);

	// start and manage game loop. On end return players end choice
	const gameEndPlayerChoice start();

	// delegate fired every new turn in loop
	delegate<void, const unsigned short> newTurnDelegate;

public: // getters
	const gameState getState() const { return this->state; }
	const unsigned short getTurn() const { return this->turn; }
	const gameMaster& getMaster() const { return *this->master.get(); }
	const combatResult& getCombatResults(const unsigned short index) const;

private:
	// test if one of the game end conditions is true
	void testGameEnd();

private:
	combatResult combatResults[constants::nbPlayers];
	unsigned short turn;
	gameState state;
	sharedCharacter characters[constants::nbPlayers];
	std::unique_ptr<gameRenderer> renderer;
	std::unique_ptr<gameMaster> master;

	// how often the dodge challenge is done
	static inline const unsigned short dodgeChallengeTurnPeriod{ 3 };
};