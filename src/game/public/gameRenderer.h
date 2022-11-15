#pragma once

#include "character.h"
#include <memory>
#include "constants.h"
#include "consoleUtil.h"

class gameWorld;

// render everything during actual game combat and gather player input every turn
class gameRenderer {
public:
	gameRenderer(sharedCharacter inCharacters[constants::nbPlayers]);

	// main render function called by game loop every turn
	void render(const gameWorld& world);

	// return true if playerIndex use his character special ability
	const bool doPlayerUseAbility(const unsigned short playerIndex);

	// called before starting game loop
	void askPlayerReady();

protected: // renders that might change every turn
	void renderCharactersHealthBar(constSharedCharacter& character);
	void renderCharacterAbility(constSharedCharacter& character);
	void renderTurn(const gameWorld& world);

protected: // renders everything that don't change whole game
	void renderStatics();
	void renderTitle();
	void renderCharactersName();
	void renderCharactersArt();

protected: // helper to align console cursor to one of the characters
	void AlignCursorToCharacter(constSharedCharacter& character);
	void AlignCursorToRightCharacter();
	void AlignCursorToLeftCharacter();

protected:
	enum class characterLineRendering {
		title = 1,
		turn,
		info = 5,
		name = 9,
		healthBar,
		ability,
		asciiArt,
		combatText,
	};

	consoleUtil console;
	constSharedCharacter characters[constants::nbPlayers];
	unsigned short rightArtWidth;
	const unsigned short characterPadding = 10;
};