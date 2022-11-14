#pragma once

#include <string>

namespace constants {
	inline constexpr unsigned short nbPlayers{ 2 };
	inline const char* const characterDatasPath{ "./data/characters/" };
	inline const char* const abilityDatasPath{ "./data/abilities/" };
	inline const char* const dataExtension{ ".json" };
	inline const std::string & vowels{ "aeiouyAEIOUY" };
}
