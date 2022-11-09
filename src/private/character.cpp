#include "character.h"
#include <fstream>

const unsigned short healthContainer::takeDamage(unsigned short amount) {
	if (this->shield >= amount) {
		this->shield -= amount;
		amount = 0;
	}
	else if (this->shield > 0) {
		amount -= this->shield;
		this->shield = 0;
	}

	this->base -= amount;

	return this->getAmount();
}

void characterData::hydrateFromJson(const Json::Value& json) {
	Json::Value tmp;
	this->healthBase = json["healthBase"].asUInt();
	this->healthShield = json["healthShield"].asUInt();
	this->asciiArtPath = json["asciiArtPath"].asString();
}

character character::make(const characterData& Data) {
	std::ifstream ifs{ Data.asciiArtPath };

	return character(healthContainer(Data.healthBase, Data.healthShield),
		std::string((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>()), Data.name);
}

character::character(healthContainer inHealth, const std::string inAsciiArt, const std::string inName)
	: health(inHealth)
	, asciiArt(inAsciiArt)
	, name(inName) {}

