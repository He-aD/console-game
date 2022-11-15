#include "character.h"
#include <fstream>

std::shared_ptr<character> character::make(const data& data, gameWorld& world) {
	std::ifstream ifs{ data.asciiArtPath };

	return std::shared_ptr<character>(new character(
		std::move(std::make_shared<abilityTargetCharacteristics>(data.characteristicData, world)),
		std::string((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>()), 
		data.name, 
		data.abilityName, 
		world));
}

character::character(std::shared_ptr<abilityTargetCharacteristics> inCharacteristics, const std::string& inAsciiArt, 
	const std::string& inName, const std::string& abilityName, gameWorld& world)
	: characteristics(inCharacteristics)
	, asciiArt(inAsciiArt)
	, name(inName) {
	
	this->ability = std::move(
		abilityFactory::make(abilityName.c_str(), world, this->characteristics)
	);
}

//////////////////////// DATA ////////////////////////////////////

void character::data::hydrateFromJson(const Json::Value& json) {
	this->characteristicData.hydrateFromJson(json["abilityTargetCharacteristics"]);
	this->asciiArtPath = json["asciiArtPath"].asString();
	this->abilityName = json["abilityName"].asString();
}

