#include "stunAbility.h"
#include "gameWorld.h"

stunAbility::stunAbility(const abilityBase::data& data, gameWorld& world,
	std::shared_ptr<abilityTargetCharacteristics> inOwnerCharacteristics)
	: abilityBase(data, "stun", world, inOwnerCharacteristics) {}

void stunAbility::_process(abilityTargetCharacteristics& characteristics) {
		this->world->getMaster().stun(characteristics);
}