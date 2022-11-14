#pragma once

#include <string>
#include <memory>
#include "ability.h"

struct characterData {
	characterData() 
		: asciiArtPath("./assets/knight.txt")
		, name("Player test")
		, abilityName("charge") {}

	void hydrateFromJson(const Json::Value& json);

	std::string asciiArtPath;
	std::string name;
	std::string abilityName;
	abilityTargetCharacteristicsData characteristicData;
};

class gameWorld;

class character {
public:
	static std::shared_ptr<character> make(const characterData& data, gameWorld& world);

	const abilityBase& getAbility() const { return *this->ability.get(); }
	abilityBase& getAbility() { return *this->ability.get(); }
	abilityTargetCharacteristics& getCharacteristics() { return *this->characteristics.get(); }
	const abilityTargetCharacteristics& getCharacteristics() const { return *this->characteristics.get(); }

public:
	std::shared_ptr<abilityTargetCharacteristics> characteristics;

	const std::string asciiArt;
	const std::string name;

protected:
	character(std::shared_ptr<abilityTargetCharacteristics> inCharacteristics, const std::string& inAsciiArt, 
		const std::string& inName, const std::string& abilityName, gameWorld& world);

	std::unique_ptr<abilityBase> ability;
};

typedef std::shared_ptr<character> sharedCharacter;
typedef std::shared_ptr<const character> constSharedCharacter;