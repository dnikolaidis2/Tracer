#pragma once

#include <glm/glm.hpp>

#include "AABB.h"
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
		double U;
		double V;
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
		virtual bool BoundingBox(double time0, double time1, AABB& outputBox) const = 0;
	};

	class Translate : public Hittable
	{
	public:
		Translate(Ref<Hittable> object, const glm::dvec3& displacement)
			: Object(object), Offset(displacement) {}

		virtual bool Hit(const Ray& r, double tMin, double tMax, HitRecord& record) const override;
		virtual bool BoundingBox(double time0, double time1, AABB& outputBox) const override;
	public:
		Ref<Hittable> Object;
		glm::dvec3 Offset;
	};

	class RotateY : public Hittable
	{
	public:
		RotateY(Ref<Hittable> object, double angle);

		virtual bool Hit(const Ray& r, double tMin, double tMax, HitRecord& record) const override;
		virtual bool BoundingBox(double time0, double time1, AABB& outputBox) const override;
	public:
		Ref<Hittable> Object;
		double SinTheta;
		double CosTheta;
		bool HasBox;
		AABB Box;
	};
}
