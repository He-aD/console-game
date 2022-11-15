#include "gameRenderer.h"
#include <iostream>
#include <vector>
#include "gameWorld.h"
#include <conio.h>
#include <algorithm>

gameRenderer::gameRenderer(sharedCharacter inCharacters[constants::nbPlayers]) {
	this->rightArtWidth = 0;

	// store player's character and render them
	for (unsigned short i = 0; i < constants::nbPlayers; i++) {
		this->characters[i] = inCharacters[i];
	}
}

void gameRenderer::askPlayerReady() {
	this->console.cursorCoordinate.Y += 3;
	this->console.renderTextXCentered("Everything is ready. Prepare to fight!!\n");

	system("pause");

	this->console.clearConsole();
}

const bool gameRenderer::doPlayerUseAbility(const unsigned short playerIndex) {
	std::ostringstream oss;
	std::string tmp;
	unsigned short lineLength = 0;

	// display which player input now
	this->console.cursorCoordinate.Y = static_cast<SHORT>(characterLineRendering::info);
	oss << "PLAYER " << playerIndex + 1 << " PLAY";
	this->console.renderTextXCentered(oss);
	this->console.cursorCoordinate.Y++;

askInput: // ask if player use his ability this turn
	oss << this->characters[playerIndex]->name << " do you use your ability this turn? y: yes | n: no";
	lineLength = (unsigned short)oss.str().length();
	this->console.renderTextXCentered(oss);
	tmp = _getch();
	
	// analyse player input and ask again if unknown
	std::transform(tmp.begin(), tmp.end(), tmp.begin(),
		[](unsigned char c) {return std::tolower(c); });
	if (tmp.find("y") == std::string::npos && tmp.find("n") == std::string::npos) {
		oss << "Oops, wrong input. ";

		goto askInput;
	}

	// remove written text
	this->console.clearX(lineLength);
	this->console.cursorCoordinate.Y--;
	this->console.clearX(lineLength);

	return tmp.find("y") != std::string::npos;
}

void gameRenderer::render(const gameWorld& world) {
	if (world.getTurn() == 1) {
		this->renderStatics();
	}

	this->renderTurn(world);

	for (auto& character : this->characters) {
		this->renderCharactersHealthBar(character);
		this->renderCharacterAbility(character);
	}
}

void gameRenderer::renderStatics() {
	this->renderTitle();
	this->renderCharactersArt();
	this->renderCharactersName();
}

void gameRenderer::renderTitle() {
	std::ostringstream oss;
	this->console.cursorCoordinate.Y = static_cast<SHORT>(characterLineRendering::title);
	oss << this->characters[0]->name << " vs " << this->characters[1]->name;

	this->console.renderTextXCentered(oss);
}

void gameRenderer::renderCharactersName() {
	this->console.cursorCoordinate.Y = static_cast<SHORT>(characterLineRendering::name);

	for (auto& character : this->characters) {
		this->AlignCursorToCharacter(character);
		std::cout << character->name << std::endl;
	}
}

void gameRenderer::renderCharactersArt() {
	size_t pos{ 0 };
	std::string s;
	const std::string delimiter{ "\n" };
	std::vector<std::string> lines;

	// calculate character art width and store each art's line in a vector and render
	for (auto& character : this->characters) {
		// calculate character art width and store each line in a vector
		s = character->asciiArt;
		lines.clear();
		while ((pos = s.find(delimiter)) != std::string::npos) {
			if (pos > this->rightArtWidth && this->console.cursorCoordinate.X > this->characterPadding) {
				this->rightArtWidth = (unsigned short)pos;
			}
			lines.push_back(s.substr(0, pos));
			s.erase(0, pos + delimiter.length());
		}

		// last character art's line
		if (s.length() > this->rightArtWidth && this->console.cursorCoordinate.X > this->characterPadding) {
			this->rightArtWidth = (unsigned short)pos;
		}
		lines.push_back(s);

		// render character
		this->console.cursorCoordinate.Y = static_cast<SHORT>(characterLineRendering::asciiArt);
		this->AlignCursorToCharacter(character);
		for (std::string line : lines) {
			this->console.setCursorPosition();
			std::cout << line << std::endl;
			this->console.cursorCoordinate.Y++;
		}
	}
}

void gameRenderer::renderCharactersHealthBar(constSharedCharacter& character) {
	constexpr unsigned short maxWidth = 17;

	// remove previous health bar
	this->console.cursorCoordinate.Y = static_cast<SHORT>(characterLineRendering::healthBar);
	this->AlignCursorToCharacter(character);
	this->console.clearX(maxWidth);

	// build new health bar
	this->AlignCursorToCharacter(character);
	std::string healthBar{ "[HP: " };
	healthBar += std::to_string(character->getCharacteristics().health.getBase());
	if (character->getCharacteristics().health.getShield() > 0) {
		healthBar += " | SH: " + std::to_string(character->getCharacteristics().health.getShield());
	}
	healthBar.append("]");

	// render health bar
	std::cout << healthBar;
}

void gameRenderer::renderCharacterAbility(constSharedCharacter& character) {
	constexpr unsigned short maxWidth = 32;

	// remove previous ability text
	this->console.cursorCoordinate.Y = static_cast<SHORT>(characterLineRendering::ability);
	this->AlignCursorToCharacter(character);
	this->console.clearX(maxWidth);
	
	// build new ability text
	this->AlignCursorToCharacter(character);
	std::ostringstream oss; 
	const abilityBase& ability = character->getAbility();
	oss << ability.name << " available ";
	if (ability.getNbTurnToBeAvailable() > 0) {
		oss << "in " << ability.getNbTurnToBeAvailable() << " turn";
	}

	// render ability text
	std::cout << oss.str();
}

void gameRenderer::renderTurn(const gameWorld& world) {
	std::ostringstream oss;

	this->console.cursorCoordinate.Y = static_cast<SHORT>(characterLineRendering::turn);
	oss << "Turn: " << world.getTurn();
	this->console.renderTextXCentered(oss);
}

void gameRenderer::AlignCursorToCharacter(constSharedCharacter& character) {
	this->characters[0] != character ? this->AlignCursorToRightCharacter() : this->AlignCursorToLeftCharacter();
}

void gameRenderer::AlignCursorToLeftCharacter() {
	this->console.cursorCoordinate.X = this->characterPadding;
	this->console.setCursorPosition();
}

void gameRenderer::AlignCursorToRightCharacter() {
	this->console.cursorCoordinate.X = this->console.consoleInfo.dwSize.X - (this->rightArtWidth + this->characterPadding);
	this->console.setCursorPosition();
}