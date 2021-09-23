#pragma once

#include "Base.h"
#include "Hittable.h"

namespace TC {
	class HittableList : public Hittable
	{
	public:
		HittableList() {}
		HittableList(Ref<Hittable> object) { Add(object); }

		void Clear() { Objects.clear(); }
		void Add(Ref<Hittable> object) { Objects.push_back(object); }

		virtual bool Hit(const Ray& r, double tMin, double tMax, HitRecord& record) const override;
	public:
		std::vector<Ref<Hittable>> Objects;
	};
}