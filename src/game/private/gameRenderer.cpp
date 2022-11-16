#include "gameRenderer.h"
#include <iostream>
#include <vector>
#include "gameWorld.h"
#include <algorithm>
#include "game.h"

gameRenderer::gameRenderer(sharedCharacter inCharacters[constants::nbPlayers]) {
	this->rightArtWidth = 0;

	// store player's character and render them
	for (unsigned short i = 0; i < constants::nbPlayers; i++) {
		this->characters[i] = inCharacters[i];
	}
}

gameRenderer::~gameRenderer() {
	this->console.clearConsole();
}

void gameRenderer::askPlayerReady() {
	this->console.cursorCoordinate.Y = static_cast<SHORT>(characterLineRendering::title);
	this->console.renderTextXCentered("Everything is ready. Prepare to fight!!\n");

	this->console.cursorCoordinate.Y++;
	this->console.renderTextXCentered("Press any key to start...", true, true);

	this->console.clearConsole();
}

const bool gameRenderer::doPlayerUseAbility(const unsigned short playerIndex) {
	std::ostringstream oss;
	std::string input;
	unsigned short lineLength{ 0 };

	// display which player input now
	this->console.cursorCoordinate.Y = static_cast<SHORT>(characterLineRendering::playerInput);
	oss << "PLAYER " << playerIndex + 1 << " PLAY";
	this->console.renderTextXCentered(oss);
	this->console.cursorCoordinate.Y++;

askInput: // ask if player use his ability this turn
	oss << "Do " << this->characters[playerIndex]->name << " use his ability this turn? y: yes | n: no";
	lineLength = (unsigned short)oss.str().length();
	input = this->console.renderTextXCentered(oss, true, true);
	
	// analyse player input and ask again if unknown
	std::transform(input.begin(), input.end(), input.begin(),
		[](unsigned char c) {return std::tolower(c); });
	if (input.find("y") == std::string::npos && input.find("n") == std::string::npos) {
		oss << "Oops, wrong input. ";

		goto askInput;
	}

	// remove written text
	this->console.clearX(lineLength);
	this->console.cursorCoordinate.Y--;
	this->console.clearX(lineLength);

	return input.find("y") != std::string::npos;
}

void gameRenderer::render(const gameWorld& world) {
	if (world.getTurn() == 1) {
		this->renderStatics();
	}

	this->renderTurnTitle(world);

	// render characters dynamic UI
	for (auto& character : this->characters) {
		this->renderCharactersHealthBar(character);
		this->renderCharacterAbility(character);
	}

	if (world.getTurn() > 1) {
		// ask player input before starting new turn: avoid automatic turn when no player can input
		// and improve readability
		this->console.cursorCoordinate.Y = static_cast<SHORT>(characterLineRendering::info);
		this->console.renderTextXCentered("Press any key to play new turn...", true, true);

		// clear previous combat text rendered
		this->console.cursorCoordinate.X = 0;
		this->console.cursorCoordinate.Y = static_cast<SHORT>(characterLineRendering::combatResult);
		for (unsigned short i = 0; i < 6; i++) {
			this->console.clearX(console.consoleInfo.dwSize.X);
			this->console.cursorCoordinate.Y++;
		}
	}
}

void gameRenderer::doDodgeChallenge(const gameWorld& world, std::vector<gameMaster::dodgeChallenge>& challenges) {
	if (challenges.size() > 0) {
		unsigned short i{ 0 }, winnerIndex{ 0 };
		std::ostringstream oss;
		std::string playerAnswer;
		double minDuration{ -1 };

		// display challenge title
		this->console.cursorCoordinate.Y = static_cast<SHORT>(characterLineRendering::playerInput);
		oss << "Dodge challenge, difficulty: " << challenges[0].getDifficulty();
		this->console.renderTextXCentered(oss);
		this->console.cursorCoordinate.Y++;

		// do challenge for each player
		for (auto& challenge : challenges) {
			// display ready go
			oss << "PLAYER " << i + 1 << " press any key to start challenge...";
			this->console.renderTextXCentered(oss, true, true);

			// clear text and start challenge chrono
			this->console.clearX(this->console.consoleInfo.dwSize.X);
			challenge.start();

		submitChallenge: // loop challenge until right answer
			oss << challenge.text << " ";
			playerAnswer = this->console.renderTextXCentered(oss, true);
			if (!challenge.isCorrectAnswer(playerAnswer)) {
				this->console.clearX(playerAnswer.length());
				oss << "Wrong! Try again: ";
				goto submitChallenge;
			}

			// refresh eventual winner
			if (minDuration == -1 || minDuration > challenge.getDuration()) {
				minDuration = challenge.getDuration();
				winnerIndex = i;
			}

			// clear challenge and display player result
			this->console.clearX(this->console.consoleInfo.dwSize.X);
			oss << "PLAYER " << i + 1 << " find the solution in: " << challenge.getDuration() << "s. ";
			this->console.renderTextXCentered(oss);

			this->console.cursorCoordinate.Y++;
			i++;
		}

		// display winner and his reward
		oss << "PLAYER " << winnerIndex + 1 << " win the challenge! " << "His character will dodge ";
		oss << world.getMaster().dodgeChallengeBonus[challenges[winnerIndex].difficulty] << "% of next damage.";
		this->console.renderTextXCentered(oss);
		
		// wait input to continue
		this->console.cursorCoordinate.Y++;
		this->console.renderTextXCentered("Press any key to end turn...", true, true);

		// clear challenges text
		for (i = 0; i < 6; i++) {
			this->console.clearX(this->console.consoleInfo.dwSize.X);
			this->console.cursorCoordinate.Y--;
		}
	}
}

void gameRenderer::renderCombatResult(const gameWorld& world) {
	unsigned short i{ 0 };
	std::ostringstream oss;

	this->console.cursorCoordinate.Y = static_cast<SHORT>(characterLineRendering::combatResult);
	for (auto& character : this->characters) {
		const gameWorld::combatResult& result = world.getCombatResults(i);

		// combat text render if player try to used his ability
		if (result.playerInputAbility) {
			oss << character->name;

			if (result.abilitySucceeded) {
				oss << " successfully used " << character->getAbility().name;
				oss << ", " << character->getAbility().message << "!";
			}
			else {
				oss << " failed to use " << character->getAbility().name << ".";
			}
			this->console.renderTextXCentered(oss);
			this->console.cursorCoordinate.Y++;
		}		
		
		// combat text render anyway
		oss << character->name;
		if (result.damageMade > 0) {
			oss << " attacked and made " << result.damageMade << " damage.";
		}
		else {
			oss << " failed his attack because " << this->characters[(i + 1) % 2]->name << " prevent it!";
		}
		this->console.renderTextXCentered(oss);
		this->console.cursorCoordinate.Y++;

		i++;
	}
}

const gameEndPlayerChoice gameRenderer::renderEndScreen(const gameWorld& world) {
	std::ostringstream oss;
	std::string tmp;

	// refresh health bar
	for (auto& character : this->characters) {
		this->renderCharactersHealthBar(character);
	}

	// display eventual winner
	const unsigned short winnerIndex = (unsigned short)world.getState();
	this->console.cursorCoordinate.Y = static_cast<SHORT>(characterLineRendering::info);
	if (winnerIndex < constants::nbPlayers) {
		oss << "PLAYER " << winnerIndex + 1 << " AS " << this->characters[winnerIndex]->name << " IS THE WINNER!";
	}
	else {
		oss << "DRAW!";
	}	
	this->console.renderTextXCentered(oss);
	this->console.cursorCoordinate.Y++;

askInput: // ask player what he wants to do
	oss << "If you want to remake press: r, make a new game: n, to quit: q ";
	tmp = this->console.renderTextXCentered(oss, true, true);

	// analyse player input and ask again if unknown
	std::transform(tmp.begin(), tmp.end(), tmp.begin(),
		[](unsigned char c) {return std::tolower(c); });
	if (tmp.find("r") == std::string::npos && tmp.find("n") == std::string::npos && 
		tmp.find("q") == std::string::npos) {
		oss << "Oops, wrong input. ";

		goto askInput;
	}

	// return enum
	if (tmp.find("r") != std::string::npos) {
		return gameEndPlayerChoice::remake;
	}
	else if (tmp.find("n") != std::string::npos) {
		return gameEndPlayerChoice::newGame;
	}
	else {
		return gameEndPlayerChoice::quit;
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

void gameRenderer::renderTurnTitle(const gameWorld& world) {
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