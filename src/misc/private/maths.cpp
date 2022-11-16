#include "maths.h"
#include <random>

namespace maths {
	short randRange(const short min, const short max) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distr(min, max);

		return (short)distr(gen);
	}
}

