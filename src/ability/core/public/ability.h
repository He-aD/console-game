#pragma once

#include <string>
#include <json/json.h>
#include "abilityTargetCharacteristics.h"
#include <memory>

struct abilityData {
	abilityData()
		: probabilityOfSuccess(50)
		, message("default ability text")
		, cooldown(2) {}

	void hydrateFromJson(const Json::Value& json);

	unsigned short probabilityOfSuccess;
	std::string message;
	unsigned short cooldown;
};

class abilityBase {
public:
	abilityBase(const abilityData& data, const std::string inName);

	virtual const bool process(const abilityTargetCharacteristics& characteristic) = 0;

	const unsigned short nbTurnToBeAvailable() const { return 0; }

	const unsigned short probabilityOfSuccess;
	const std::string message;
	const unsigned short cooldown;
	const std::string name;
};

class abilityFactory {
public:
	static std::unique_ptr<abilityBase> make(const char* abilityName);
};