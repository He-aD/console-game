#pragma once

#include "character.h"
#include <memory>
#include "constants.h"
#include "consoleUtil.h"

class gameRenderer {
public:
	gameRenderer(sharedCharacter inCharacters[constants::nbPlayers]);

	void render();

protected:
	enum class characterLineRendering {
		title = 1,
		name,
		healthBar,
		asciiArt,
		combatText,
	};

protected:
	void renderStatics();
	void renderTitle();
	void renderCharactersName();
	void renderCharactersArt();
	void renderCharactersHealthBar(constSharedCharacter character);

	void AlignCursorToRightCharacter();
	void AlignCursorToLeftCharacter();

protected:
	consoleUtil console;
	constSharedCharacter characters[constants::nbPlayers];
	unsigned short rightArtWidth;
};