#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/norm.hpp>

#include "Base.h"

namespace TC {
	struct RendererStats
	{
		std::atomic<int> PixelsRendered;
	};

	void WriteColor(uint8_t* buffer, glm::dvec3 color, int samplesPerPixel)
	{
		auto r = color.r;
		auto g = color.g;
		auto b = color.b;

		// Divide the color by the number of samples.
		auto scale = 1.0 / samplesPerPixel;
		r = std::sqrt(scale * r);
		g = std::sqrt(scale * g);
		b = std::sqrt(scale * b);

		// Write the translated [0,255] value of each color component.
		buffer[0] = static_cast<int>(256 * glm::clamp(r, 0.0, 0.999));
		buffer[1] = static_cast<int>(256 * glm::clamp(g, 0.0, 0.999));
		buffer[2] = static_cast<int>(256 * glm::clamp(b, 0.0, 0.999));
	}
}
