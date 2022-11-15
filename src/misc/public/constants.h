#pragma once

#include <string>

namespace constants {
	struct rectangle {
		rectangle(const unsigned short inX, const unsigned short inY, const unsigned short inWidth,
			const unsigned short inHeight)
			: x(inX)
			, y(inY)
			, width(inWidth)
			, height(inHeight) {}

		const unsigned short x;
		const unsigned short y;
		const unsigned short width;
		const unsigned short height;
	};

	inline constexpr unsigned short nbPlayers{ 2 };
	inline const char* const characterDatasPath{ "./data/characters/" };
	inline const char* const abilityDatasPath{ "./data/abilities/" };
	inline const char* const dataExtension{ ".json" };
	inline const std::string & vowels{ "aeiouyAEIOUY" };
	inline const rectangle consolePosition{ 300, 200, 800, 500 };
}
