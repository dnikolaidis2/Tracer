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
}
