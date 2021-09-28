#pragma once

#include "Base.h"
#include "Hittable.h"
#include "Material.h"


namespace TC {
	class ConstantMedium : public Hittable
	{
	public:
		ConstantMedium(Ref<Hittable> object, double density, Ref<Texture> color)
			: Boundary(object), NegativeInverseDensity(-1 / density), PhaseFunction(CreateRef<Isotropic>(color)) {}

		ConstantMedium(Ref<Hittable> object, double density, glm::dvec3 color)
			: Boundary(object), NegativeInverseDensity(-1 / density), PhaseFunction(CreateRef<Isotropic>(color)) {}

		virtual bool Hit(const Ray& r, double tMin, double tMax, HitRecord& record) const override;
		virtual bool BoundingBox(double time0, double time1, AABB& outputBox) const override;
	public:
		Ref<Hittable> Boundary;
		Ref<Material> PhaseFunction;
		double NegativeInverseDensity;
	};
}
