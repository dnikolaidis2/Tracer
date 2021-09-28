#include "AARect.h"

namespace TC {
	bool XYRect::Hit(const Ray& r, double tMin, double tMax, HitRecord& record) const
	{
		auto t = (K - r.Origin.z) / r.Direction.z;
		if (t < tMin || t > tMax)
			return false;

		auto x = r.Origin.x + t * r.Direction.x;
		auto y = r.Origin.y + t * r.Direction.y;

		if (x < X0 || x > X1 || y < Y0 || y > Y1)
			return false;

		record.U = (x - X0) / (X1 - X0);
		record.V = (y - Y0) / (Y1 - Y0);
		record.T = t;
		auto outwardNormal = glm::dvec3(0.0, 0.0, 1.0);
		record.SetFaceNormal(r, outwardNormal);
		record.Material = Material;
		record.Point = r.At(t);
		return true;
	}

	bool XYRect::BoundingBox(double time0, double time1, AABB& outputBox) const
	{
		outputBox = AABB(glm::dvec3(X0, Y0, K - 0.0001), glm::dvec3(X1, Y1, K + 0.0001));
		return  true;
	}

	bool XZRect::Hit(const Ray& r, double tMin, double tMax, HitRecord& record) const
	{
		auto t = (K - r.Origin.y) / r.Direction.y;
		if (t < tMin || t > tMax)
			return false;

		auto x = r.Origin.x + t * r.Direction.x;
		auto z = r.Origin.z + t * r.Direction.z;

		if (x < X0 || x > X1 || z < Z0 || z > Z1)
			return false;

		record.U = (x - X0) / (X1 - X0);
		record.V = (z - Z0) / (Z1 - Z0);
		record.T = t;
		auto outwardNormal = glm::dvec3(0.0, 1.0, 0.0);
		record.SetFaceNormal(r, outwardNormal);
		record.Material = Material;
		record.Point = r.At(t);
		return true;
	}

	bool XZRect::BoundingBox(double time0, double time1, AABB& outputBox) const
	{
		outputBox = AABB(glm::dvec3(X0, K - 0.0001, Z0), glm::dvec3(X1, K + 0.0001, Z1));
		return  true;
	}

	bool YZRect::Hit(const Ray& r, double tMin, double tMax, HitRecord& record) const
	{
		auto t = (K - r.Origin.x) / r.Direction.x;
		if (t < tMin || t > tMax)
			return false;

		auto y = r.Origin.y + t * r.Direction.y;
		auto z = r.Origin.z + t * r.Direction.z;

		if (y < Y0 || y > Y1 || z < Z0 || z > Z1)
			return false;

		record.U = (y - Y0) / (Y1 - Y0);
		record.V = (z - Z0) / (Z1 - Z0);
		record.T = t;
		auto outwardNormal = glm::dvec3(1.0, 0.0, 0.0);
		record.SetFaceNormal(r, outwardNormal);
		record.Material = Material;
		record.Point = r.At(t);
		return true;
	}

	bool YZRect::BoundingBox(double time0, double time1, AABB& outputBox) const
	{
		outputBox = AABB(glm::dvec3(K - 0.0001, Y0, Z0), glm::dvec3(K + 0.0001, Y1, Z1));
		return  true;
	}
}
