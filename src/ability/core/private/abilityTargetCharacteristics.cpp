#include "abilityTargetCharacteristics.h"
#include "gameWorld.h"

abilityTargetCharacteristics::abilityTargetCharacteristics(const abilityTargetCharacteristics::data& data, gameWorld& world)
	: health(healthContainer(data.healthBase, data.healthShield))
	, attackPower(variable<unsigned short>(data.attackPower, world.newTurnDelegate))
	, stun(variable<bool>(false, world.newTurnDelegate)) {}

//////////////////////// DATA ////////////////////////////////////

void abilityTargetCharacteristics::data::hydrateFromJson(const Json::Value& json) {
	this->healthBase = json["healthBase"].asUInt();
	this->healthShield = json["healthShield"].asUInt();
	this->attackPower = json["attackPower"].asUInt();
}
