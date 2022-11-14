#pragma once

#include "health.h"
#include <json/json.h>

struct abilityTargetCharacteristicsData {
	abilityTargetCharacteristicsData()
		: healthBase(10)
		, healthShield(5)
		, attackPower(2) {}

	void hydrateFromJson(const Json::Value& json);

	unsigned short healthBase;
	unsigned short healthShield;
	unsigned short attackPower;
};

class abilityTargetCharacteristics final {
public:
	abilityTargetCharacteristics(const abilityTargetCharacteristicsData& data);

	healthContainer health;

	unsigned short attackPower;
};