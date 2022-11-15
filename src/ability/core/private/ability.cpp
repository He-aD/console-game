#include "ability.h"
#include "chargeAbility.h"
#include "stunAbility.h"
#include "constants.h"
#include <fstream> 
#include "gameWorld.h"
#include <random>

abilityBase::abilityBase(const abilityBase::data& data, const std::string inName, gameWorld& inWorld,
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
	if (this->getNbTurnToBeAvailable() == 0 && this->doCastSucceed()) {
		this->_process(characteristics);
		this->startCooldown();

		return true;
	}

	return false;
}

void abilityBase::startCooldown() {
	this->nbTurnToBeAvailable = this->cooldown + 1;
}

void abilityBase::onNewTurn(const unsigned short turn) {
	if (this->nbTurnToBeAvailable > 0) {
		this->nbTurnToBeAvailable--;
	}
}

const bool abilityBase::doCastSucceed() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distr(0, 100);
	const unsigned short random = (unsigned short)distr(gen);

	return random <= this->probabilityOfSuccess;
}

//////////////////////// FACTORY ////////////////////////////////////

std::unique_ptr<abilityBase> abilityFactory::make(const char* abilityName, gameWorld& world,
	std::shared_ptr<abilityTargetCharacteristics> inOwnerCharacteristic) {
	abilityBase::data data{};
	
	// try to read ability json data file
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

	// instanciate proper ability
	if (strcmp(abilityName, "charge") == 0) {
		return std::make_unique<chargeAbility>(std::move(data), world, std::move(inOwnerCharacteristic));
	}
	else if (strcmp(abilityName, "stun") == 0) {
		return std::make_unique<stunAbility>(std::move(data), world, std::move(inOwnerCharacteristic));
	}

notFound:
	return std::make_unique<chargeAbility>(std::move(data), world, std::move(inOwnerCharacteristic));
}

//////////////////////// DATA ////////////////////////////////////

void abilityBase::data::hydrateFromJson(const Json::Value& json) {
	this->probabilityOfSuccess = json["probabilityOfSuccess"].asUInt();
	this->message = json["message"].asString();
	this->cooldown = json["cooldown"].asUInt();
}