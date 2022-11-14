#pragma once

#include "ability.h"

class chargeAbility final : public abilityBase {
public:
	chargeAbility(const abilityData& data);

	virtual const bool process(const abilityTargetCharacteristics& characteristic);
};