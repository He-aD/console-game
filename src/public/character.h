#pragma once

#include <string>
#include <memory>

class healthContainer {
public:
	healthContainer(const unsigned short inBase, const unsigned short inShield = 0)
		: base(inBase)
		, shield(inShield) {}

	// return shield + base amount
	const unsigned short takeDamage(unsigned short amount);

	const unsigned short getBase() const { return this->base; }
	const unsigned short getShield() const { return this->shield; }

	// consider the base and shield as health. Indeed shield as no expiration time or different rules as base amount.
	const unsigned short getAmount() const { return this->base + this->shield; }

protected:
	unsigned short base;
	unsigned short shield;
};

struct characterData {
	characterData() 
		: asciiArtPath("./assets/knight.txt")
		, name("Player test")
		, healthBase(10)
		, healthShield(5) {}

	characterData(const std::string inAsciiArtPath, const std::string inName, const unsigned short inHealthBase,
		const unsigned short inHealthShield)
		: asciiArtPath(inAsciiArtPath)
		, name(inName)
		, healthBase(inHealthBase)
		, healthShield(inHealthShield) {}

	std::string asciiArtPath;
	std::string name;
	unsigned short healthBase;
	unsigned short healthShield;
};

class character {
public:
	static character make(const characterData& Data);

	character(healthContainer inHealth, const std::string inAsciiArt, const std::string inName);

	healthContainer health;

	const std::string asciiArt;
	const std::string name;
};

typedef std::shared_ptr<character> sharedCharacter;
typedef std::shared_ptr<const character> constSharedCharacter;