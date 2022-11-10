#pragma once

#include "character.h"
#include <memory>
#include "constants.h"
#include "consoleUtil.h"

class gameWorld;

class gameRenderer {
public:
	gameRenderer(sharedCharacter inCharacters[constants::nbPlayers]);

	void render(const gameWorld& world);

	// return true if playerIndex use his character special ability
	const bool doPlayerUseAbility(const unsigned short playerIndex);

	void askPlayerReady();

protected:
	void renderCharactersHealthBar(constSharedCharacter character);
	void renderCharacterAbility(constSharedCharacter character);
	void renderTurn(const gameWorld& world);

protected:
	void renderStatics();
	void renderTitle();
	void renderCharactersName();
	void renderCharactersArt();

protected:
	void AlignCursorToCharacter(constSharedCharacter character);
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