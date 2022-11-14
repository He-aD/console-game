#include "chargeAbility.h"
#include "gameWorld.h"

chargeAbility::chargeAbility(const abilityData& data, gameWorld& world, 
	std::shared_ptr<abilityTargetCharacteristics> inOwnerCharacteristics)
	: abilityBase(data, "charge", world, inOwnerCharacteristics) {
	
}

const bool chargeAbility::process(abilityTargetCharacteristics& characteristics) {
	if (this->doCastSucceed() && this->getNbTurnToBeAvailable() == 0) {
		this->world->getMaster().increaseAttackPower(*this->ownerCharacteristics, 2);
		this->startCooldown();

		return true;
	}

	return false;
}