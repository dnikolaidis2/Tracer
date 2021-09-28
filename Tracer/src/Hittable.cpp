#include "Hittable.h"

#include <glm/gtx/string_cast.hpp>

namespace TC {
	bool Translate::Hit(const Ray& r, double tMin, double tMax, HitRecord& record) const
	{
		Ray movedRay(r.Origin - Offset, r.Direction, r.Time);
		if (!Object->Hit(movedRay, tMin, tMax, record))
			return false;

		record.Point += Offset;
		record.SetFaceNormal(movedRay, record.Normal);

		return true;
	}

	bool Translate::BoundingBox(double time0, double time1, AABB& outputBox) const
	{
		if (!Object->BoundingBox(time0, time1, outputBox))
			return false;

		outputBox = AABB(outputBox.Min + Offset, outputBox.Max + Offset);
		return true;
	}

	RotateY::RotateY(Ref<Hittable> object, double angle)
		: Object(object)
	{
		auto radians = glm::radians(angle);
		SinTheta = sin(radians);
		CosTheta = cos(radians);
		HasBox = Object->BoundingBox(0, 1, Box);

		glm::dvec3 min(infinity);
		glm::dvec3 max(-infinity);

		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				for (int k = 0; k < 2; k++) {
					auto x = i * Box.Max.x + (1 - i) * Box.Min.x;
					auto y = j * Box.Max.y + (1 - j) * Box.Min.y;
					auto z = k * Box.Max.z + (1 - k) * Box.Min.z;

					auto newx = CosTheta * x + SinTheta * z;
					auto newz = -SinTheta * x + CosTheta* z;

					glm::dvec3 tester(newx, y, newz);

					for (int c = 0; c < 3; c++) {
						min[c] = fmin(min[c], tester[c]);
						max[c] = fmax(max[c], tester[c]);
					}
				}
			}
		}

		Box = AABB(min, max);
	}

	bool RotateY::Hit(const Ray& r, double tMin, double tMax, HitRecord& record) const
	{
		auto origin = r.Origin;
		auto direction = r.Direction;

		origin.x = CosTheta * r.Origin.x - SinTheta * r.Origin.z;
		origin.z = SinTheta * r.Origin.x + CosTheta * r.Origin.z;

		direction.x = CosTheta * r.Direction.x - SinTheta * r.Direction.z;
		direction.z = SinTheta * r.Direction.x + CosTheta * r.Direction.z;

		Ray rotatedRay(origin, direction, r.Time);

		if (!Object->Hit(rotatedRay, tMin, tMax, record))
			return false;

		auto p = record.Point;
		auto normal = record.Normal;

		p.x = CosTheta * record.Point.x + SinTheta * record.Point.z;
		p.z = -SinTheta * record.Point.x + CosTheta * record.Point.z;

		normal.x = CosTheta * record.Normal.x + SinTheta * record.Normal.z;
		normal.z = -SinTheta * record.Normal.x + CosTheta * record.Normal.z;

		record.Point = p;
		record.SetFaceNormal(rotatedRay, normal);

		return true;
	}

	bool RotateY::BoundingBox(double time0, double time1, AABB& outputBox) const
	{
		outputBox = Box;
		return HasBox;
	}
}
