#include "gameMenu.h"
#include <Windows.h>
#include <iostream>
#include <filesystem>
#include <fstream> 

gameMenu::gameMenu() {
	this->console.cursorCoordinate.Y = 1;
	this->console.renderTextXCentered("### Welcome to Tristan's Job Interview Game! ###");
	this->console.cursorCoordinate.Y += 2;

	this->fectchAllCharactersLabel();
}

gameMenu::~gameMenu() {
	this->console.clearConsole();
}

const std::array<character::data, constants::nbPlayers> gameMenu::gatherPlayerCharacterData() {
	std::array<character::data, constants::nbPlayers> characterDatas{ character::data{}, character::data{} };
	unsigned short i{ 0 };
	std::ostringstream oss;
	std::string tmp;
	bool find = false;
	
	// ask player inputs to configure their character
	for (i = 0; i < constants::nbPlayers; i++) {
		// display characters name and ask for choice
		oss << "Player " << i + 1 << " please choose your character. Press ";
		for (auto& name : this->charactersLabel) {
			oss << name[0] << " for " << name << ", ";
		}
		oss.seekp(oss.str().length() - 2);
		oss << ":";
		tmp = this->console.renderTextXCentered(oss, true);
		this->console.cursorCoordinate.Y++;

		// read player input and look for a match in characters name
		std::transform(tmp.begin(), tmp.end(), tmp.begin(),
			[](unsigned char c) {return std::tolower(c); });
		find = false;
		for (auto& label : this->charactersLabel) {
			if (*tmp.c_str() == label[0]) {
				// hydrate character data from json
				this->hydrateCharacterData(characterDatas[i], label);

				// display player character's selection
				oss << "Player " << i + 1 << " choose a";
				if (constants::vowels.find(label[0]) != std::string::npos) {
					oss << "n";
				}
				oss << " " << label << "!";
				this->console.renderTextXCentered(oss);

				find = true;
				break;
			}
		}

		// unrecognized player input ask again
		if (!find) {
			this->console.renderTextXCentered("Oops, I did not understand your choice. Try again.");
			i--;
			this->console.cursorCoordinate.Y++;
			continue;
		}
		this->console.cursorCoordinate.Y++;

		// ask player to name its character
		oss << "Player " << i + 1 << " please enter a name for your character: ";
		characterDatas[i].name = this->console.renderTextXCentered(oss, true);
		this->console.cursorCoordinate.Y += 2;
	}

	return characterDatas;
}

void gameMenu::fectchAllCharactersLabel() {
	namespace fs = std::filesystem;
	std::string tmp;

	for (auto& p : fs::directory_iterator(constants::characterDatasPath)) {
		tmp = p.path().u8string();
		tmp.erase(0, strlen(constants::characterDatasPath));
		tmp.erase(tmp.find(constants::dataExtension), strlen(constants::dataExtension));

		this->charactersLabel.push_back(tmp);
	}
}

const bool gameMenu::hydrateCharacterData(character::data& data, const std::string& characterLabel) {
	const std::string& filePath = constants::characterDatasPath + characterLabel + constants::dataExtension;
	Json::Value json;
	std::ifstream ifs{ filePath };

	try {
		ifs >> json;
		data.hydrateFromJson(json);

		return true;
	}
	catch (const Json::RuntimeError) {
		return false;
	}
}

