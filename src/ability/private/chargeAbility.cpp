#include "chargeAbility.h"

chargeAbility::chargeAbility(const abilityData& data) 
	: abilityBase(data, "charge") {

}

const bool chargeAbility::process(const abilityTargetCharacteristics& characteristic) {
	return true;
}