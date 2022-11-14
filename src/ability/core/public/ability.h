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

class gameWorld;

class abilityBase {
public:
	abilityBase(const abilityData& data, const std::string inName, gameWorld& inWorld, 
		std::shared_ptr<abilityTargetCharacteristics> inOwnerCharacteristics);

	virtual const bool process(abilityTargetCharacteristics& characteristics) = 0;

	const unsigned short getNbTurnToBeAvailable() const { return this->nbTurnToBeAvailable; }

	const unsigned short probabilityOfSuccess;
	const std::string message;
	const unsigned short cooldown;
	const std::string name;

protected:
	void startCooldown();
	void onNewTurn(const unsigned short turn);
	const bool doCastSucceed();

	gameWorld* world;
	std::shared_ptr<abilityTargetCharacteristics> ownerCharacteristics;
	unsigned short nbTurnToBeAvailable;
};

class abilityFactory {
public:
	static std::unique_ptr<abilityBase> make(const char* abilityName, gameWorld& world,
		std::shared_ptr<abilityTargetCharacteristics> inOwnerCharacteristics);
};