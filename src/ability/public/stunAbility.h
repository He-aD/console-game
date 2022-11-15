#pragma once

#include "ability.h"

class stunAbility final : public abilityBase {
public:
	stunAbility(const abilityData& data, gameWorld& world, std::shared_ptr<abilityTargetCharacteristics> inOwnerCharacteristic);

	virtual void _process(abilityTargetCharacteristics& characteristics);
};