#include "ConstantMedium.h"

#include <glm/gtx/string_cast.hpp>

namespace TC
{
	bool ConstantMedium::Hit(const Ray& r, double tMin, double tMax, HitRecord& record) const
	{
		// Print occassional samples when debugging. To enable, set enableDebug true.
		const bool enableDebug = false;
		const bool debugging = enableDebug && glm::linearRand(0.0, 1.0) < 0.00001;

		HitRecord record1, record2;

		if (!Boundary->Hit(r, -infinity, infinity, record1))
			return false;

		if (!Boundary->Hit(r, record1.T + 0.0001, infinity, record2))
			return false;

		if (debugging) std::cerr << "\nt_min=" << record1.T << ", t_max=" << record2.T << '\n';

		if (record1.T < tMin) record1.T = tMin;
		if (record2.T > tMax) record1.T = tMax;

		if (record1.T >= record2.T)
			return false;

		if (record1.T < 0)
			record1.T = 0;

		const auto rayLength = glm::length(r.Direction);
		const auto distanceInsideBoundary = (record2.T - record1.T) * rayLength;
		const auto hitDistance = NegativeInverseDensity * log(glm::linearRand(0.0, 1.0));

		if (hitDistance > distanceInsideBoundary)
			return false;

		record.T = record1.T + hitDistance / rayLength;
		record.Point = r.At(record.T);

		if (debugging)
			std::cerr << "hit_distance = " << hitDistance << '\n'
			<< "rec.t = " << record.T << '\n'
			<< "rec.p = " << glm::to_string(record.Point) << '\n';

		record.Normal = glm::dvec3(1.0, 0.0, 0.0); // arbitrary
		record.FrontFace = true; // also arbitrary
		record.Material = PhaseFunction;

		return true;
	}

	bool ConstantMedium::BoundingBox(double time0, double time1, AABB& outputBox) const
	{
		return Boundary->BoundingBox(time0, time1, outputBox);
	}
}