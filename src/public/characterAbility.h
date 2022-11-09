#pragma once

#include <string>

class characterAbility {
public:
	characterAbility(const std::string& inName);

	const unsigned short GetCooldown() const { return 3; }

	const std::string name;
};