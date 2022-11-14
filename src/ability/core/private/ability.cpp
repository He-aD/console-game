#include "ability.h"
#include "chargeAbility.h"
#include "stunAbility.h"
#include "constants.h"
#include <fstream> 

void abilityData::hydrateFromJson(const Json::Value& json) {
	this->probabilityOfSuccess = json["probabilityOfSuccess"].asUInt();
	this->message = json["message"].asString();
	this->cooldown = json["cooldown"].asUInt();
}

abilityBase::abilityBase(const abilityData& data, const std::string inName)
	: probabilityOfSuccess(data.probabilityOfSuccess)
	, message(data.message)
	, cooldown(data.cooldown)
	, name(inName) {}

std::unique_ptr<abilityBase> abilityFactory::make(const char* abilityName) {
	abilityData data{};
	const std::string& filePath = constants::abilityDatasPath + std::string(abilityName) + constants::dataExtension;
	Json::Value json;
	std::ifstream ifs{ filePath };

	try {
		ifs >> json;
		data.hydrateFromJson(json["common"]);
	}
	catch (const Json::RuntimeError) {
		goto notFound;
	}

	if (strcmp(abilityName, "charge") == 0) {
		return std::make_unique<chargeAbility>(chargeAbility(std::move(data)));
	}
	else if (strcmp(abilityName, "stun") == 0) {
		return std::make_unique<stunAbility>(stunAbility(std::move(data)));
	}

notFound:
	return std::make_unique<chargeAbility>(chargeAbility(std::move(data)));
}