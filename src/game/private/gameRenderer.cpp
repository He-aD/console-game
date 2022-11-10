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

	this->console.cursorCoordinate.Y = static_cast<SHORT>(characterLineRendering::info);
	oss << "PLAYER " << playerIndex + 1 << " PLAY";
	this->console.renderTextXCentered(oss);
	this->console.cursorCoordinate.Y++;

askInput:
	oss << this->characters[playerIndex]->name << " do you use your ability this turn? y: yes | n: no";
	lineLength = (unsigned short)oss.str().length();
	this->console.renderTextXCentered(oss);
	
	tmp = _getch();
	std::transform(tmp.begin(), tmp.end(), tmp.begin(),
		[](unsigned char c) {return std::tolower(c); });
	if (tmp.find("y") == std::string::npos && tmp.find("n") == std::string::npos) {
		oss << "Oops, wrong input. ";

		goto askInput;
	}

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

	// render player1 name
	this->AlignCursorToLeftCharacter();
	std::cout << this->characters[0]->name << std::endl;

	// render player2 name at console extreme right 
	this->AlignCursorToRightCharacter();
	std::cout << this->characters[1]->name << std::endl;
}

void gameRenderer::renderCharactersArt() {
	size_t pos{ 0 };
	std::string s;
	const std::string delimiter{ "\n" };
	std::vector<std::string> lines;

	// calculate player art width and store each art's line in a vector and render
	for (auto& character : this->characters) {
		s = character->asciiArt;
		lines.clear();
		while ((pos = s.find(delimiter)) != std::string::npos) {
			if (pos > this->rightArtWidth && this->console.cursorCoordinate.X > this->characterPadding) {
				this->rightArtWidth = (unsigned short)pos;
			}
			lines.push_back(s.substr(0, pos));
			s.erase(0, pos + delimiter.length());
		}

		if (s.length() > this->rightArtWidth && this->console.cursorCoordinate.X > this->characterPadding) {
			this->rightArtWidth = (unsigned short)pos;
		}
		lines.push_back(s);

		this->console.cursorCoordinate.Y = static_cast<SHORT>(characterLineRendering::asciiArt);
		this->AlignCursorToCharacter(character);
		for (std::string line : lines) {
			this->console.SetCursorPosition();
			std::cout << line << std::endl;
			this->console.cursorCoordinate.Y++;
		}
	}
}

void gameRenderer::renderCharactersHealthBar(constSharedCharacter character) {
	constexpr unsigned short maxWidth = 17;

	this->console.cursorCoordinate.Y = static_cast<SHORT>(characterLineRendering::healthBar);
	this->AlignCursorToCharacter(character);
	this->console.clearX(maxWidth);
	this->AlignCursorToCharacter(character);

	std::string healthBar{ "[HP: " };
	healthBar += std::to_string(character->health.getBase());
	if (character->health.getShield() > 0) {
		healthBar += " | SH: " + std::to_string(character->health.getShield());
	}
	healthBar.append("]");

	std::cout << healthBar;
}

void gameRenderer::renderCharacterAbility(constSharedCharacter character) {
	constexpr unsigned short maxWidth = 32;
	std::ostringstream oss; 
	const characterAbility& ability = character->GetAbility();

	oss << ability.name << " available ";
	if (ability.GetCooldown() > 0) {
		oss << "in " << ability.GetCooldown() << " turn";
	}

	this->console.cursorCoordinate.Y = static_cast<SHORT>(characterLineRendering::ability);
	this->AlignCursorToCharacter(character);
	this->console.clearX(maxWidth);
	this->AlignCursorToCharacter(character);

	std::cout << oss.str();
}

void gameRenderer::renderTurn(const gameWorld& world) {
	std::ostringstream oss;

	this->console.cursorCoordinate.Y = static_cast<SHORT>(characterLineRendering::turn);
	oss << "Turn: " << world.getTurn();
	this->console.renderTextXCentered(oss);
}

void gameRenderer::AlignCursorToCharacter(constSharedCharacter character) {
	this->characters[0] != character ? this->AlignCursorToRightCharacter() : this->AlignCursorToLeftCharacter();
}

void gameRenderer::AlignCursorToLeftCharacter() {
	this->console.cursorCoordinate.X = this->characterPadding;
	this->console.SetCursorPosition();
}

void gameRenderer::AlignCursorToRightCharacter() {
	this->console.cursorCoordinate.X = this->console.consoleInfo.dwSize.X - (this->rightArtWidth + this->characterPadding);
	this->console.SetCursorPosition();
}