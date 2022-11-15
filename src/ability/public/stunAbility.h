#pragma once

#include "ability.h"

class stunAbility final : public abilityBase {
public:
	stunAbility(const abilityBase::data& data, const std::string& inName, gameWorld& world,
		std::shared_ptr<abilityTargetCharacteristics> inOwnerCharacteristic);

	virtual void _process(abilityTargetCharacteristics& characteristics);
};