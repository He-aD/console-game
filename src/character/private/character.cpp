#include "character.h"
#include <fstream>

void characterData::hydrateFromJson(const Json::Value& json) {
	this->characteristicData.hydrateFromJson(json["abilityTargetCharacteristics"]);
	this->asciiArtPath = json["asciiArtPath"].asString();
	this->abilityName = json["abilityName"].asString();
}

std::shared_ptr<character> character::make(const characterData& data) {
	std::ifstream ifs{ data.asciiArtPath };
	abilityTargetCharacteristics characteristics { data.characteristicData };

	return std::shared_ptr<character>(new character(std::move(characteristics),
		std::string((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>()), data.name, data.abilityName));
}

character::character(abilityTargetCharacteristics inCharacteristics, const std::string& inAsciiArt, const std::string& inName,
	const std::string& abilityName)
	: characteristics(inCharacteristics)
	, asciiArt(inAsciiArt)
	, name(inName) {
	this->ability = std::move(abilityFactory::make(abilityName.c_str()));
}

