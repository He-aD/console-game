#pragma once

#include <string>
#include <memory>
#include "ability.h"

class gameWorld;

// represent a player character, act as a container do not define itself any game rule logic
class character {
public: // factory
	struct data { // character base data
		data()
			: asciiArtPath("./assets/knight.txt")
			, name("Player test")
			, abilityName("charge") {}

		void hydrateFromJson(const Json::Value& json);

		std::string asciiArtPath;
		std::string name;
		std::string abilityName;
		abilityTargetCharacteristicsData characteristicData;
	};

	static std::shared_ptr<character> make(const data& data, gameWorld& world);

public: // getters
	const abilityBase& getAbility() const { return *this->ability.get(); }
	abilityBase& getAbility() { return *this->ability.get(); }

	const abilityTargetCharacteristics& getCharacteristics() const { return *this->characteristics.get(); }
	abilityTargetCharacteristics& getCharacteristics() { return *this->characteristics.get(); }
	
public: // const and public attributes
	const std::string asciiArt;
	const std::string name;

	// attributes mutable by game rules (eg: stun)
	std::shared_ptr<abilityTargetCharacteristics> characteristics;

protected:
	character(std::shared_ptr<abilityTargetCharacteristics> inCharacteristics, const std::string& inAsciiArt, 
		const std::string& inName, const std::string& abilityName, gameWorld& world);

	std::unique_ptr<abilityBase> ability;
};

typedef std::shared_ptr<character> sharedCharacter;
typedef std::shared_ptr<const character> constSharedCharacter;