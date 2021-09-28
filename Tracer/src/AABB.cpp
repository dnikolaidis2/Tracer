#include "AABB.h"

#include <glm/common.hpp>
#include "glm/ext/vector_common.hpp"

namespace TC
{
	bool AABB::Hit(const Ray& r, double tMin, double tMax) const
	{
        for (int a = 0; a < 3; a++) {
            auto invD = 1.0f / r.Direction[a];
            auto t0 = (Min[a] - r.Origin[a]) * invD;
            auto t1 = (Max[a] - r.Origin[a]) * invD;
            if (invD < 0.0f)
                std::swap(t0, t1);
            tMin = t0 > tMin ? t0 : tMin;
            tMax = t1 < tMax ? t1 : tMax;
            if (tMax <= tMin)
                return false;
        }
        return true;
	}

	AABB AABB::SurroundingBox(AABB box0, AABB box1)
	{
        glm::dvec3 small = glm::fmin(box0.Min, box1.Min);
        glm::dvec3 big = glm::fmax(box0.Max, box1.Max);
        return AABB(small, big);
	}
}
