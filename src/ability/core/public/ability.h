#pragma once

#include <string>
#include <json/json.h>
#include "abilityTargetCharacteristics.h"
#include <memory>

class gameWorld;

// abstract of abilities
class abilityBase {
public: // construct
	struct data { // ability base data
		data()
			: probabilityOfSuccess(50)
			, message("default ability text")
			, cooldown(2) {}

		void hydrateFromJson(const Json::Value& json);

		unsigned short probabilityOfSuccess;
		std::string message;
		unsigned short cooldown;
	};

	abilityBase(const abilityBase::data& data, const std::string& inName, gameWorld& inWorld, 
		std::shared_ptr<abilityTargetCharacteristics> inOwnerCharacteristics);

public: // called by external classes to process ability logic. Call _process implemented by actual ability
	const bool process(abilityTargetCharacteristics& characteristics);

protected: // to be implemented by actual ability, contain their logic
	virtual void _process(abilityTargetCharacteristics& characteristics) = 0;

public: // getter
	const unsigned short getNbTurnToBeAvailable() const { return this->nbTurnToBeAvailable; }

protected: // cooldowns utils	
	// test ability's probability
	const bool doCastSucceed();

	// called when ability cast succeed
	void startCooldown();

	// bind to gameWorld new turn delegate: decrease nbTurnToBeAvailable every turn
	void onNewTurn(const unsigned short turn);

public: // const attributes
	const unsigned short probabilityOfSuccess;
	const std::string message;
	const unsigned short cooldown;
	const std::string name;

protected: // attributes
	gameWorld* world;
	std::shared_ptr<abilityTargetCharacteristics> ownerCharacteristics;
	unsigned short nbTurnToBeAvailable;
};

// ability factory: instanciate proper ability based on name and json data files
class abilityFactory {
public:
	static std::unique_ptr<abilityBase> make(const char* abilityName, gameWorld& world,
		std::shared_ptr<abilityTargetCharacteristics> inOwnerCharacteristics);
};