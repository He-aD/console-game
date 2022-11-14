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

class character {
public:
	static std::shared_ptr<character> make(const characterData& data);

	const abilityBase& GetAbility() const { return *this->ability.get(); }

public:
	abilityTargetCharacteristics characteristics;

	const std::string asciiArt;
	const std::string name;

protected:
	character(abilityTargetCharacteristics inCharacteristics, const std::string& inAsciiArt, const std::string& inName,
		const std::string& abilityName);

	std::unique_ptr<abilityBase> ability;
};

typedef std::shared_ptr<character> sharedCharacter;
typedef std::shared_ptr<const character> constSharedCharacter;