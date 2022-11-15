#include "chargeAbility.h"
#include "gameWorld.h"
#include <fstream> 

chargeAbility::chargeAbility(const abilityBase::data& data, const std::string& inName, gameWorld& world,
	std::shared_ptr<abilityTargetCharacteristics> inOwnerCharacteristics)
	: abilityBase(data, inName, world, inOwnerCharacteristics) {

	// try to read json data file
	const std::string& filePath = constants::abilityDatasPath + this->name + constants::dataExtension;
	Json::Value json;
	std::ifstream ifs{ filePath };
	try {
		ifs >> json;
		this->attackPowerMultiplier = json["attackPowerMultiplier"].asUInt();
	}
	catch (const Json::RuntimeError) {
		this->attackPowerMultiplier = 0;
	}
}

void chargeAbility::_process(abilityTargetCharacteristics& characteristics) {
	this->world->getMaster().increaseAttackPower(*this->ownerCharacteristics, this->attackPowerMultiplier);
}