#pragma once

#include "abilityTargetCharacteristics.h"

// define all game unitary actions logic
// mainly called by abilites process functions
class gameMaster {
public:
	void increaseAttackPower(abilityTargetCharacteristics& target, const unsigned short multiplier) const;

	void stun(abilityTargetCharacteristics& target) const;

	const bool tryMakeDamage(const abilityTargetCharacteristics& source, abilityTargetCharacteristics& target) const;
};