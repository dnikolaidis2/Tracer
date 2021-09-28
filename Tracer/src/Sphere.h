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
		virtual bool BoundingBox(double time0, double time1, AABB& outputBox) const override;
		static void GetSphereUV(const glm::dvec3& point, double& u, double& v);
	public:
		glm::dvec3 Center;
		double Radius;
		Ref<Material> Material;
	};

	class MovingSphere : public Hittable
	{
	public:
		MovingSphere() {}
		MovingSphere(glm::dvec3 startCenter, glm::dvec3 endCenter, double startTime, double endTime, double r, Ref<Material> material)
			: StartCenter(startCenter), EndCenter(endCenter), StartTime(startTime), EndTime(endTime), Radius(r), Material(material)
		{};

		glm::dvec3 Center(double time) const;

		virtual bool Hit(const Ray& r, double tMin, double tMax, HitRecord& record) const override;
		virtual bool BoundingBox(double time0, double time1, AABB& outputBox) const override;
	public:
		glm::dvec3 StartCenter, EndCenter;
		double StartTime, EndTime, Radius;
		Ref<Material> Material;
	};

}