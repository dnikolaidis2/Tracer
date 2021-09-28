#pragma once

#include "Base.h"
#include "Hittable.h"
#include "HittableList.h"

namespace TC {

	class BVHNode : public Hittable
	{
	public:
		BVHNode();
		BVHNode(const HittableList& list, double time0, double time1)
			: BVHNode(list.Objects, 0, list.Objects.size(), time0, time1)
		{}

		BVHNode(const std::vector<Ref<Hittable>>& srcObjects, size_t start, size_t end, double time0, double time1);

		virtual bool Hit(const Ray& r, double tMin, double tMax, HitRecord& record) const override;
		virtual bool BoundingBox(double time0, double time1, AABB& outputBox) const override;

		static bool BoxCompare(const Ref<Hittable> a, const Ref<Hittable> b, int axis);
		static bool BoxXCompare(const Ref<Hittable> a, const Ref<Hittable> b);
		static bool BoxYCompare(const Ref<Hittable> a, const Ref<Hittable> b);
		static bool BoxZCompare(const Ref<Hittable> a, const Ref<Hittable> b);
	public:
		Ref<Hittable> Left;
		Ref<Hittable> Right;
		AABB Box;
	};
}