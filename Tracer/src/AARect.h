#pragma once

#include "Hittable.h"

namespace TC {
	class XYRect : public Hittable
	{
	public:
		XYRect() {}
		XYRect(double x0, double x1, double y0, double y1, double k, Ref<Material> material)
			: X0(x0), X1(x1), Y0(y0), Y1(y1), K(k), Material(material) {}

		virtual bool Hit(const Ray& r, double tMin, double tMax, HitRecord& record) const override;
		virtual bool BoundingBox(double time0, double time1, AABB& outputBox) const override;
	public:
		Ref<Material> Material;
		double X0, X1, Y0, Y1, K;
	};

	class XZRect : public Hittable
	{
	public:
		XZRect() {}
		XZRect(double x0, double x1, double z0, double z1, double k, Ref<Material> material)
			: X0(x0), X1(x1), Z0(z0), Z1(z1), K(k), Material(material) {}

		virtual bool Hit(const Ray& r, double tMin, double tMax, HitRecord& record) const override;
		virtual bool BoundingBox(double time0, double time1, AABB& outputBox) const override;
	public:
		Ref<Material> Material;
		double X0, X1, Z0, Z1, K;
	};

	class YZRect : public Hittable
	{
	public:
		YZRect() {}
		YZRect(double y0, double y1, double z0, double z1, double k, Ref<Material> material)
			: Y0(y0), Y1(y1), Z0(z0), Z1(z1), K(k), Material(material) {}

		virtual bool Hit(const Ray& r, double tMin, double tMax, HitRecord& record) const override;
		virtual bool BoundingBox(double time0, double time1, AABB& outputBox) const override;
	public:
		Ref<Material> Material;
		double Y0, Y1, Z0, Z1, K;
	};
}
