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
		GetSphereUV(outwardNormal, record.U, record.V);
		record.Material = Material;

		return true;
	}

	bool Sphere::BoundingBox(double time0, double time1, AABB& outputBox) const
	{
		outputBox = AABB(Center - glm::dvec3(Radius), Center + glm::dvec3(Radius));
		return true;
	}

	void Sphere::GetSphereUV(const glm::dvec3& point, double& u, double& v)
	{
		// point: a given point on the sphere of radius one, centered at the origin.
		// u: returned value [0,1] of angle around the Y axis from X=-1.
		// v: returned value [0,1] of angle from Y=-1 to Y=+1.
		//     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
		//     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
		//     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

		auto theta = acos(-point.y);
		auto phi = atan2(-point.z, point.x) + pi;

		u = phi / (2 * pi);
		v = theta / pi;
	}

	glm::dvec3 MovingSphere::Center(double time) const
	{
		return StartCenter + ((time - StartTime) / (EndTime - StartTime)) * (EndCenter - StartCenter);
	}

	bool MovingSphere::Hit(const Ray& r, double tMin, double tMax, HitRecord& record) const
	{
		glm::dvec3 oc = r.Origin - Center(r.Time);
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
		glm::dvec3 outwardNormal = (record.Point - Center(r.Time)) / Radius;
		record.SetFaceNormal(r, outwardNormal);
		record.Material = Material;

		return true;
	}

	bool MovingSphere::BoundingBox(double time0, double time1, AABB& outputBox) const
	{
		AABB box0(Center(time0) - glm::dvec3(Radius), Center(time1) + glm::dvec3(Radius));
		AABB box1(Center(time1) - glm::dvec3(Radius), Center(time1) + glm::dvec3(Radius));
		outputBox = AABB::SurroundingBox(box0, box1);
		return true;
	}
}
