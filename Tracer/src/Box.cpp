#include "Box.h"

namespace TC
{
	Box::Box(const glm::dvec3& point0, const glm::dvec3& point1, Ref<Material> material)
	{
		BoxMin = point0;
		BoxMax = point1;

		Sides.Add(CreateRef<XYRect>(point0.x, point1.x, point0.y, point1.y, point1.z, material));
		Sides.Add(CreateRef<XYRect>(point0.x, point1.x, point0.y, point1.y, point0.z, material));

		Sides.Add(CreateRef<XZRect>(point0.x, point1.x, point0.z, point1.z, point1.y, material));
		Sides.Add(CreateRef<XZRect>(point0.x, point1.x, point0.z, point1.z, point0.y, material));

		Sides.Add(CreateRef<YZRect>(point0.y, point1.y, point0.z, point1.z, point1.x, material));
		Sides.Add(CreateRef<YZRect>(point0.y, point1.y, point0.z, point1.z, point0.x, material));

	}

	bool Box::Hit(const Ray& r, double tMin, double tMax, HitRecord& record) const
	{
		return Sides.Hit(r, tMin, tMax, record);
	}

	bool Box::BoundingBox(double time0, double time1, AABB& outputBox) const
	{
		outputBox = AABB(BoxMin, BoxMax);
		return true;
	}
}
