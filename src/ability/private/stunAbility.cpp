#include "stunAbility.h"
#include "gameWorld.h"

stunAbility::stunAbility(const abilityBase::data& data, const std::string& inName, gameWorld& world,
	std::shared_ptr<abilityTargetCharacteristics> inOwnerCharacteristics)
	: abilityBase(data, inName, world, inOwnerCharacteristics) {}

void stunAbility::_process(abilityTargetCharacteristics& characteristics) {
		this->world->getMaster().stun(characteristics);
}