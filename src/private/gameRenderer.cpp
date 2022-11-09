#include "gameRenderer.h"
#include <iostream>
#include <vector>

gameRenderer::gameRenderer(sharedCharacter inCharacters[constants::nbPlayers]) {
	this->rightArtWidth = 0;

	// store player's character and render them
	for (unsigned short i = 0; i < constants::nbPlayers; i++) {
		this->characters[i] = inCharacters[i];
	}

	this->console.cursorCoordinate.Y += 3;
	this->console.renderTextXCentered("Everything is ready. Prepare to fight!!\n");
	system("pause");
	this->console.clearConsole();

	this->renderCharactersArtAndName();
	this->render();
}

void gameRenderer::render() {
	for (auto character : this->characters) {
		this->renderCharacterHealthBar(character);
	}
}

void gameRenderer::renderCharactersArtAndName() {
	// render player1 name
	this->AlignCursorToLeftCharacter();
	this->console.cursorCoordinate.Y = static_cast<SHORT>(characterLineRendering::name);
	this->console.SetCursorPosition();
	std::cout << this->characters[0]->name << std::endl;

	// render player1 art
	this->console.cursorCoordinate.Y = static_cast<SHORT>(characterLineRendering::asciiArt);
	this->console.SetCursorPosition();
	std::cout << this->characters[0]->asciiArt << std::endl;

	// calculate player2 art width and store each art's line in a vector
	size_t pos{ 0 };
	std::string s{ this->characters[1]->asciiArt };
	const std::string delimiter{ "\n" };
	std::vector<std::string> lines;

	while ((pos = s.find(delimiter)) != std::string::npos) {
		if (pos > this->rightArtWidth) {
			this->rightArtWidth = (unsigned short)pos;
		}
		lines.push_back(s.substr(0, pos));
		s.erase(0, pos + delimiter.length());
	}

	if (s.length() > this->rightArtWidth) {
		this->rightArtWidth = (unsigned short)pos;
	}
	lines.push_back(s);

	// render player2 name at console extreme right 
	this->AlignCursorToRightCharacter();
	this->console.cursorCoordinate.Y = static_cast<SHORT>(characterLineRendering::name);
	this->console.SetCursorPosition();
	std::cout << this->characters[1]->name << std::endl;

	// render player2 art at console extreme right 
	this->console.cursorCoordinate.Y = static_cast<SHORT>(characterLineRendering::asciiArt);
	for (std::string line : lines) {
		this->console.SetCursorPosition();
		std::cout << line << std::endl;
		this->console.cursorCoordinate.Y++;
	}
}

void gameRenderer::renderCharacterHealthBar(constSharedCharacter character) {
	this->console.cursorCoordinate.Y = static_cast<SHORT>(characterLineRendering::healthBar);
	this->characters[0] != character ? this->AlignCursorToRightCharacter() : this->AlignCursorToLeftCharacter();
	this->console.SetCursorPosition();

	std::string healthBar{ "[HP: " };
	healthBar += std::to_string(character->health.getBase());
	if (character->health.getShield() > 0) {
		healthBar += " | SH: " + std::to_string(character->health.getShield());
	}
	healthBar.append("]");

	std::cout << healthBar << std::endl;

	this->console.cursorCoordinate.Y = 30;
	this->console.SetCursorPosition();
}

void gameRenderer::AlignCursorToLeftCharacter() {
	this->console.cursorCoordinate.X = 0;
}

void gameRenderer::AlignCursorToRightCharacter() {
	this->console.cursorCoordinate.X = this->console.consoleInfo.dwSize.X - this->rightArtWidth;
}