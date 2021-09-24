#include "Sphere.h"

#include <glm/gtx/norm.hpp>

namespace TC {
	bool Sphere::Hit(const Ray& r, double tMin, double tMax, HitRecord& record) const
	{
		glm::dvec3 oc = r.Origin - Center;
		auto a = glm::length2(r.Direction);
		auto halfB = glm::dot(oc, r.Direction);
		auto c = glm::length2(oc) - Radius * Radius;

		auto discriminant = halfB * halfB - a * c;
		if (discriminant < 0) return false;
		auto sqrtd = std::sqrt(discriminant);

		// Find the nearest root that lies in the acceptable range
		auto root = (-halfB - sqrtd) / a;
		if (root < tMin || tMax < root)
		{
			root = (-halfB + sqrtd) / a;
			if (root < tMin || tMax < root)
			{
				return false;
			}
		}

		record.T = root;
		record.Point = r.At(record.T);
		glm::dvec3 outwardNormal = (record.Point - Center) / Radius;
		record.SetFaceNormal(r, outwardNormal);
		record.Material = Material;

		return true;
	}
}
