#pragma once

#include <glm/glm.hpp>

#include "Base.h"

namespace TC {
	void WriteColor(uint8_t* buffer, glm::dvec3 color, int samplesPerPixel)
	{
		auto r = color.r;
		auto g = color.g;
		auto b = color.b;

		// Divide the color by the number of samples.
		auto scale = 1.0 / samplesPerPixel;
		r *= scale;
		g *= scale;
		b *= scale;

		// Write the translated [0,255] value of each color component.
		buffer[0] = static_cast<int>(256 * glm::clamp(r, 0.0, 0.999));
		buffer[1] = static_cast<int>(256 * glm::clamp(g, 0.0, 0.999));
		buffer[2] = static_cast<int>(256 * glm::clamp(b, 0.0, 0.999));
	}

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
