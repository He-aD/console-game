#include "stunAbility.h"

stunAbility::stunAbility(const abilityData& data)
	: abilityBase(data, "stun") {

}

const bool stunAbility::process(const abilityTargetCharacteristics& characteristic) {
	return true;
}