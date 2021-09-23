#pragma once

#include "Base.h"

namespace TC {
	inline double RandomDouble() {
		static std::uniform_real_distribution<double> distribution(0.0, 1.0);
		static std::mt19937 generator;
		return distribution(generator);
	}


	inline double RandomDouble(double min, double max) {
		// Returns a random real in [min,max).
		return min + (max - min) * RandomDouble();
	}
}
