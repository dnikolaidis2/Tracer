#pragma once

#include <glm/glm.hpp>

#include "Base.h"
#include "Ray.h"

namespace TC {
	class Material;


	struct HitRecord
	{
		glm::dvec3 Point;
		glm::dvec3 Normal;
		Ref<Material> Material;
		double T;
		bool FrontFace;

		inline void SetFaceNormal(const Ray& r, const glm::dvec3& outwardNormal)
		{
			FrontFace = glm::dot(r.Direction, outwardNormal) < 0;
			Normal = FrontFace ? outwardNormal : -outwardNormal;
		}
	};

	class Hittable
	{
	public:
		virtual bool Hit(const Ray& r, double tMin, double tMax, HitRecord& record) const = 0;
	};
}
