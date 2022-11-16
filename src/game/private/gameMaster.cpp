#include "gameMaster.h"
#include "maths.h"

void gameMaster::increaseAttackPower(abilityTargetCharacteristics& target, const unsigned short multiplier) const {
	target.attackPower *= multiplier;
}

void gameMaster::increaseDodgeFactor(abilityTargetCharacteristics& target, const unsigned short percent) const {
	const float factor = (100.f - static_cast<float>(percent)) / 100.f ;
	target.dodgeFactor *= factor;
}

void gameMaster::stun(abilityTargetCharacteristics& target) const {
	target.stun = true;
}

const unsigned short gameMaster::tryMakeDamage(const abilityTargetCharacteristics& source, abilityTargetCharacteristics& target) const {
	if (!source.stun) {
		const unsigned short damage{ static_cast<unsigned short>(source.attackPower * target.dodgeFactor) };
		target.health.takeDamage(damage);

		return damage;
	}

	return 0;
}

std::vector<gameMaster::dodgeChallenge> gameMaster::generateDodgeChallenges(
	std::array<std::shared_ptr<abilityTargetCharacteristics>, constants::nbPlayers> targets) const {
	std::vector<dodgeChallenge> challenges;
	const unsigned int difficulty = maths::randRange(0, dodgeChallengeMaxDifficulty);
	short answer{ 0 }, temp{ 0 };

	// generate a different challenge for each player
	for (auto& target : targets) {
		std::string text;
		const unsigned short nbIteration = difficulty == dodgeChallengeMaxDifficulty ? 4 : 2;

		// calculate answer to challenge and generate text operations
		for (unsigned short j = 0; j < nbIteration; j++) {
			const short number = maths::randRange(-10, 10);

			if (j > 0 && (difficulty == dodgeChallengeMaxDifficulty && j == 2 || difficulty == 0)) { // addition
				if (difficulty == dodgeChallengeMaxDifficulty) { // first number of second multiplication
					temp = number;
				}
				else {
					answer += number;
				}

				text += " + ";
				text += std::to_string(number);
			}
			else if (j > 0) { // multiplication
				text += " x " + std::to_string(number);

				if (j == 1) { // first multiplication
					answer *= number;
				}
				else { // second multiplication
					answer += temp * number;
				}
			}
			else { // first iteration
				answer = number;
				text = std::to_string(number);
			}
		}
		text += " = ?";

		challenges.push_back(gameMaster::dodgeChallenge(answer, text, difficulty, target));
	}

	return challenges;
}

void gameMaster::applyDodgeChallengeBonus(std::vector<gameMaster::dodgeChallenge>& challenges) const {
	short i{ 0 }, challengeIndex { -1 };
	double shortestReactionTime{ 0 };

	// find the challenge's winner
	for (auto& challenge : challenges) {
		if (shortestReactionTime == 0.f || shortestReactionTime > challenge.getDuration()) {
			shortestReactionTime = challenge.getDuration();
			challengeIndex = i;
		}

		i++;
	}

	// if there is a winner apply bonus
	if (challengeIndex >= 0) {
		this->increaseDodgeFactor(
			challenges[challengeIndex].getCharacteristic(),
			dodgeChallengeBonus[challenges[challengeIndex].difficulty]);
	}
}

//////////////////////// DODGE CHALLENGE ////////////////////////////////////

gameMaster::dodgeChallenge::dodgeChallenge()
	: correctAnswer(0)
	, text("")
	, difficulty(0)
	, duration(0) {}

gameMaster::dodgeChallenge::dodgeChallenge(const short inCorrectAnswer, const std::string& inText,
	const unsigned short inDifficulty, std::shared_ptr<abilityTargetCharacteristics> inCharacteristic)
	: correctAnswer(inCorrectAnswer)
	, text(inText)
	, difficulty(inDifficulty)
	, duration(0)
	, characteristic(inCharacteristic) {}

void gameMaster::dodgeChallenge::start() {
	this->startTime = std::chrono::system_clock::now();
}

const std::string gameMaster::dodgeChallenge::getDifficulty() const {
	switch (this->difficulty) {
	case 0:
		return "easy";

	case 1:
		return "medium";

	default:
		return "hard";
	}
}

const bool gameMaster::dodgeChallenge::isCorrectAnswer(const std::string& answer) {
	short playerAnswer{ 0 };

	try {
		playerAnswer = static_cast<short>(stoi(answer));
	}
	catch (const std::logic_error) {
		return false;
	}

	const bool isCorrect = this->correctAnswer == playerAnswer;

	if (isCorrect) {
		std::chrono::duration<double> elapsedSeconds = std::chrono::system_clock::now() - this->startTime;
		this->duration = elapsedSeconds.count();
	}
	
	return isCorrect;
}