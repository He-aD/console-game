#pragma once

#include "ability.h"

class chargeAbility final : public abilityBase {
public:
	chargeAbility(const abilityBase::data& data, const std::string& inName, gameWorld& world,
		std::shared_ptr<abilityTargetCharacteristics> inOwnerCharacteristic);

	virtual void _process(abilityTargetCharacteristics& characteristics);

private:
	unsigned short attackPowerMultiplier;
};