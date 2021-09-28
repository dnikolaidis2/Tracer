#pragma once

#include "AARect.h"
#include "HittableList.h"

namespace TC {
	class Box : public Hittable
	{
	public:
		Box() {}
		Box(const glm::dvec3& point0, const glm::dvec3& point1, Ref<Material> material);

		virtual bool Hit(const Ray& r, double tMin, double tMax, HitRecord& record) const override;
		virtual bool BoundingBox(double time0, double time1, AABB& outputBox) const override;
	public:
		glm::dvec3 BoxMin;
		glm::dvec3 BoxMax;
		HittableList Sides;
	};
}
