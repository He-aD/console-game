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
		name = 2,
		healthBar,
		asciiArt,
	};

protected:
	void renderCharactersArtAndName();
	void renderCharacterHealthBar(constSharedCharacter character);

	void AlignCursorToRightCharacter();
	void AlignCursorToLeftCharacter();

protected:
	consoleUtil console;
	constSharedCharacter characters[constants::nbPlayers];
	unsigned short rightArtWidth;
};