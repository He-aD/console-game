#include "gameMenu.h"
#include <Windows.h>
#include <iostream>

gameMenu::gameMenu() {
	this->console.cursorCoordinate.Y = 1;
	this->console.renderTextXCentered("### Welcome to Tristan's Job Interview Game! ###");
	this->console.cursorCoordinate.Y += 2;
}

const std::array<characterData, constants::nbPlayers> gameMenu::gatherCharacterDatas() {
	std::array<characterData, constants::nbPlayers> characterDatas{ characterData{}, characterData{} };
	unsigned short i{ 0 };
	std::ostringstream oss;
	std::string tmp;
	
	for (i = 0; i < constants::nbPlayers; i++) {
		oss << "Player " << i + 1 << " please enter your nickname: ";
		characterDatas[i].name = this->console.renderTextXCentered(oss, true);
		this->console.cursorCoordinate.Y++;
	}

	for (i = 0; i < constants::nbPlayers; i++) {
		oss << "Player " << i + 1 << " please choose your character. Press o for orc, k for knight: ";
		tmp = this->console.renderTextXCentered(oss, true);
		this->console.cursorCoordinate.Y++;

		if (tmp == "o") {
			characterDatas[i].asciiArtPath = "./assets/orc.txt";
			oss << "Player " << i + 1 << " choose an Orc!";
			this->console.renderTextXCentered(oss);
		}
		else if (tmp == "k") {
			characterDatas[i].asciiArtPath = "./assets/knight.txt";
			oss << "Player " << i + 1 << " choose a Knight!";
			this->console.renderTextXCentered(oss);
		}
		else {
			this->console.renderTextXCentered("Oops, I did not understand your choice. Try again.");
			i--;
		}
		this->console.cursorCoordinate.Y++;
	}
	//this->cursorCoordinate.Y += 3;
	//this->renderTextXCentered("Everything is ready. Prepare to fight!!\n");
	//system("pause");

	return characterDatas;
}

gameMenu::~gameMenu() {
	this->console.clearConsole();
}