#pragma once

#include <glm/glm.hpp>

#include "Hittable.h"


namespace TC {

	class Sphere : public Hittable
	{
	public:
		Sphere() {}
		Sphere(glm::dvec3 center, double r)
			: Center(center), Radius(r)
		{};

		virtual bool Hit(const Ray& r, double tMin, double tMax, HitRecord& record) const override;
	public:
		glm::dvec3 Center;
		double Radius;
	};

}