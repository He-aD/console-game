#include "abilityTargetCharacteristics.h"

void abilityTargetCharacteristicsData::hydrateFromJson(const Json::Value& json) {
	this->healthBase = json["healthBase"].asUInt();
	this->healthShield = json["healthShield"].asUInt();
	this->attackPower = json["attackPower"].asUInt();
}

abilityTargetCharacteristics::abilityTargetCharacteristics(const abilityTargetCharacteristicsData& data) 
	: health(healthContainer(data.healthBase, data.healthShield))
	, attackPower(data.attackPower) {}