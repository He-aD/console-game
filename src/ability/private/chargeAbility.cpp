#include "chargeAbility.h"
#include "gameWorld.h"

chargeAbility::chargeAbility(const abilityBase::data& data, gameWorld& world,
	std::shared_ptr<abilityTargetCharacteristics> inOwnerCharacteristics)
	: abilityBase(data, "charge", world, inOwnerCharacteristics) {}

void chargeAbility::_process(abilityTargetCharacteristics& characteristics) {
	this->world->getMaster().increaseAttackPower(*this->ownerCharacteristics, 2);
}