#include "gameMaster.h"

void gameMaster::increaseAttackPower(abilityTargetCharacteristics& target, const unsigned short multiplier) const {
	target.attackPower *= multiplier;
}

void gameMaster::stun(abilityTargetCharacteristics& target) const {
	target.stun = true;
}

const unsigned short gameMaster::tryMakeDamage(const abilityTargetCharacteristics& source, abilityTargetCharacteristics& target) const {
	if (!source.stun) {
		target.health.takeDamage(source.attackPower);

		return source.attackPower;
	}

	return 0;
}