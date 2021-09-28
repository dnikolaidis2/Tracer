#pragma once

#include <glm/glm.hpp>

#include "Base.h"
#include "Ray.h"

namespace TC {
	class AABB
	{
	public:
		AABB() {}
		AABB(const glm::dvec3& min, const glm::dvec3& max) : Min(min), Max(max) {}

		bool Hit(const Ray& r, double tMin, double tMax) const;
		static AABB SurroundingBox(AABB box0, AABB box1);
	public:
		glm::dvec3 Min;
		glm::dvec3 Max;
	};
}
