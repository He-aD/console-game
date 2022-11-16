#include "ability.h"
#include "chargeAbility.h"
#include "stunAbility.h"
#include "constants.h"
#include <fstream> 
#include "gameWorld.h"
#include "maths.h"

abilityBase::abilityBase(const abilityBase::data& data, const std::string& inName, gameWorld& inWorld,
	std::shared_ptr<abilityTargetCharacteristics> inOwnerCharacteristics)
	: probabilityOfSuccess(data.probabilityOfSuccess)
	, message(data.message)
	, cooldown(data.cooldown)
	, name(inName)
	, world(&inWorld)
	, ownerCharacteristics(inOwnerCharacteristics)
	, nbTurnToBeAvailable(0) {

	this->world->newTurnDelegate.add(std::bind(
		&abilityBase::onNewTurn,
		this,
		std::placeholders::_1));
}

const bool abilityBase::process(abilityTargetCharacteristics& characteristics) {
	bool didProcess = false;

	if (this->getNbTurnToBeAvailable() == 0) {
		if (this->cast()) {
			this->_process(characteristics);

			didProcess = true;
		}
		this->startCooldown();
	}

	return didProcess;
}

void abilityBase::startCooldown() {
	this->nbTurnToBeAvailable = this->cooldown;
}

void abilityBase::onNewTurn(const unsigned short turn) {
	if (this->nbTurnToBeAvailable > 0) {
		this->nbTurnToBeAvailable--;
	}
}

const bool abilityBase::cast() {
	return maths::randRange(0, 100) <= this->probabilityOfSuccess;
}

//////////////////////// FACTORY ////////////////////////////////////

std::unique_ptr<abilityBase> abilityFactory::make(const char* abilityName, gameWorld& world,
	std::shared_ptr<abilityTargetCharacteristics> inOwnerCharacteristic) {
	abilityBase::data data{};
	const std::string name{ abilityName };
		
	// try to read ability json data file
	const std::string& filePath = constants::abilityDatasPath + name + constants::dataExtension;
	Json::Value json;
	std::ifstream ifs{ filePath };
	try {
		ifs >> json;
		data.hydrateFromJson(json["common"]);
	}
	catch (const Json::RuntimeError) {
		goto notFound;
	}

	// instanciate proper ability
	if (strcmp(abilityName, "charge") == 0) {
		return std::make_unique<chargeAbility>(std::move(data), name, world, std::move(inOwnerCharacteristic));
	}
	else if (strcmp(abilityName, "stun") == 0) {
		return std::make_unique<stunAbility>(std::move(data), name, world, std::move(inOwnerCharacteristic));
	}

notFound:
	return std::make_unique<chargeAbility>(std::move(data), name, world, std::move(inOwnerCharacteristic));
}

//////////////////////// DATA ////////////////////////////////////

void abilityBase::data::hydrateFromJson(const Json::Value& json) {
	this->probabilityOfSuccess = json["probabilityOfSuccess"].asUInt();
	this->message = json["message"].asString();
	this->cooldown = json["cooldown"].asUInt();
}