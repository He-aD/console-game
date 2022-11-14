#pragma once

#include "ability.h"

class stunAbility final : public abilityBase {
public:
	stunAbility(const abilityData& data);

	virtual const bool process(const abilityTargetCharacteristics& characteristic);
};