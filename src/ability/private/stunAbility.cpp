#include "stunAbility.h"
#include "gameWorld.h"

stunAbility::stunAbility(const abilityData& data, gameWorld& world, 
	std::shared_ptr<abilityTargetCharacteristics> inOwnerCharacteristics)
	: abilityBase(data, "stun", world, inOwnerCharacteristics) {
	
}

const bool stunAbility::process(abilityTargetCharacteristics& characteristics) {
	if (this->doCastSucceed() && this->getNbTurnToBeAvailable() == 0) {
		this->world->getMaster().stun(characteristics);
		this->startCooldown();

		return true;
	}
	
	return false;
}