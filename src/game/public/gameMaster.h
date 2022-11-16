#pragma once

#include "abilityTargetCharacteristics.h"
#include <vector>
#include <string>
#include "constants.h"
#include <array>
#include <chrono>

// define all game unitary actions logic
// mainly called by abilites process functions
class gameMaster {
public:
	void increaseAttackPower(abilityTargetCharacteristics& target, const unsigned short multiplier) const;

	// percent: 0-100
	void increaseDodgeFactor(abilityTargetCharacteristics& target, const unsigned short percent) const;

	void stun(abilityTargetCharacteristics& target) const;
	
	// return damage made bv source to target
	const unsigned short tryMakeDamage(const abilityTargetCharacteristics& source, abilityTargetCharacteristics& target) const;

public: // dodge challenge
	struct dodgeChallenge { // dodge challenge data structure
		dodgeChallenge();
		dodgeChallenge(const short inCorrectAnswer, const std::string& inText, const unsigned short inDifficulty,
			std::shared_ptr<abilityTargetCharacteristics> inCharacteristic);

		// start the chrono 
		void start();

		const std::string getDifficulty() const;
		abilityTargetCharacteristics& getCharacteristic() { return *(this->characteristic.get()); }
		const double getDuration() const { return this->duration; }

		const bool isCorrectAnswer(const std::string& answer);

		const short correctAnswer;
		const std::string text;
		// 0: easy, 1: medium, 2: hard
		const unsigned short difficulty;

	private:
		// player duration in seconds to find right answer
		double duration;
		std::shared_ptr<abilityTargetCharacteristics> characteristic;
		std::chrono::time_point<std::chrono::system_clock> startTime;
	};

	// return a dodge challenge for each target 
	std::vector<dodgeChallenge> generateDodgeChallenges(
		std::array<std::shared_ptr<abilityTargetCharacteristics>, constants::nbPlayers> targets) const;

	// define challenge winner and apply proper bonus regards to difficulty
	void applyDodgeChallengeBonus(std::vector<dodgeChallenge>& challenges) const;

	static inline const unsigned short dodgeChallengeMaxDifficulty{ 2 };
	static inline const unsigned short dodgeChallengeBonus[dodgeChallengeMaxDifficulty + 1]{ 10, 15, 20 };
};