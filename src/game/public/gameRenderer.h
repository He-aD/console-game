#pragma once

#include "character.h"
#include <memory>
#include "constants.h"
#include "consoleUtil.h"


class gameWorld;
enum class gameEndPlayerChoice;

// render everything during actual game combat and gather player input every turn
class gameRenderer {
public:
	gameRenderer(sharedCharacter inCharacters[constants::nbPlayers]);

	// clear console
	~gameRenderer();

	// main render function called by game loop beginning of every turn
	void render(const gameWorld& world);

	// render combat info called by game loop end of every turn
	void renderCombatResult(const gameWorld& world);

	// render function called when combat end ask and return player whish to quit, remake or new game
	const gameEndPlayerChoice renderEndScreen(const gameWorld& world);

	// return true if playerIndex use his character special ability
	const bool doPlayerUseAbility(const unsigned short playerIndex);

	// called before starting game loop
	void askPlayerReady();

protected: // renders that might change every turn
	void renderCharactersHealthBar(constSharedCharacter& character);
	void renderCharacterAbility(constSharedCharacter& character);
	void renderTurnTitle(const gameWorld& world);

protected: // renders everything that don't change whole game
	void renderStatics();
	void renderTitle();
	void renderCharactersName();
	void renderCharactersArt();

protected: // helpers to align console cursor to one of the UI characters
	void AlignCursorToCharacter(constSharedCharacter& character);
	void AlignCursorToRightCharacter();
	void AlignCursorToLeftCharacter();

protected:
	enum class characterLineRendering { // console Y cursor position values regards to UI element
		title,
		turn,
		combatResult = 3,
		playerInput,
		info = 8,
		name = 11,
		healthBar,
		ability,
		asciiArt,
		combatText,
	};

	consoleUtil console;
	constSharedCharacter characters[constants::nbPlayers];
	const unsigned short characterPadding = 20;

	// store UI right's character ascii art width to render it properly
	unsigned short rightArtWidth;
};