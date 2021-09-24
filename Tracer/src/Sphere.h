#pragma once

#include <glm/glm.hpp>

#include "Base.h"
#include "Hittable.h"

namespace TC {

	class Sphere : public Hittable
	{
	public:
		Sphere() {}
		Sphere(glm::dvec3 center, double r, Ref<Material> material)
			: Center(center), Radius(r), Material(material)
		{};

		virtual bool Hit(const Ray& r, double tMin, double tMax, HitRecord& record) const override;
	public:
		glm::dvec3 Center;
		double Radius;
		Ref<Material> Material;
	};

}