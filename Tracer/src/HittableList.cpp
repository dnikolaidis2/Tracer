#include "HittableList.h"

namespace TC {
	bool HittableList::Hit(const Ray& r, double tMin, double tMax, HitRecord& record) const
	{
		HitRecord temp;
		bool hitAnything = false;
		auto minT = tMax;

		for (const auto& object : Objects)
		{
			if (object->Hit(r, tMin, minT, temp))
			{
				hitAnything = true;
				minT = temp.T;
				record = temp;
			}
		}

		return hitAnything;
	}

	bool HittableList::BoundingBox(double time0, double time1, AABB& outputBox) const
	{
		if (Objects.empty()) return false;

		AABB tempBox;
		bool firstBox = true;

		for (const auto& object : Objects)
		{
			if (!object->BoundingBox(time0, time1, tempBox)) return false;
			outputBox = firstBox ? tempBox : AABB::SurroundingBox(outputBox, tempBox);
			firstBox = false;
		}

		return true;
	}
}
