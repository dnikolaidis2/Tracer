#include "BVH.h"

#include <glm/gtc/random.hpp>

namespace TC
{
	BVHNode::BVHNode(const std::vector<Ref<Hittable>>& srcObjects, size_t start, size_t end, double time0, double time1)
	{
		auto objects = srcObjects; // Create a modifiable array of the source scene objects

		int axis = glm::linearRand(0, 2);
		auto comparator = (axis == 0) ? BVHNode::BoxXCompare
						: (axis == 1) ? BVHNode::BoxYCompare
									  : BVHNode::BoxZCompare;

		size_t objectSpan = end - start;

		if (objectSpan == 1)
		{
			Left = Right = objects[start];
		}
		else if (objectSpan == 2)
		{
			if (comparator(objects[start], objects[start + 1]))
			{
				Left = objects[start];
				Right = objects[start + 1];
			}
			else
			{
				Right = objects[start];
				Left = objects[start + 1];
			}
		}
		else
		{
			std::sort(objects.begin() + start, objects.begin() + end, comparator);

			auto mid = start + objectSpan / 2;
			Left = CreateRef<BVHNode>(objects, start, mid, time0, time1);
			Right = CreateRef<BVHNode>(objects, mid, end, time0, time1);
		}

		AABB boxLeft, boxRight;
		if (!Left->BoundingBox(time0, time1, boxLeft) || !Right->BoundingBox(time0, time1, boxRight))
			std::cerr << "No bounding box in BVHNode constructor.\n";

		Box = AABB::SurroundingBox(boxLeft, boxRight);
	}

	bool BVHNode::Hit(const Ray& r, double tMin, double tMax, HitRecord& record) const
	{
		if (!Box.Hit(r, tMin, tMax))
			return false;

		bool hitLeft = Left->Hit(r, tMin, tMax, record);
		bool hitRight = Right->Hit(r, tMin, hitLeft ? record.T : tMax, record);

		return hitLeft || hitRight;
	}

	bool BVHNode::BoundingBox(double time0, double time1, AABB& outputBox) const
	{
		outputBox = Box;
		return true;
	}

	bool BVHNode::BoxCompare(const Ref<Hittable> a, const Ref<Hittable> b, int axis)
	{
		AABB boxA, boxB;

		if (!a->BoundingBox(0, 0, boxA) || !b->BoundingBox(0, 0, boxB))
			std::cerr << "No bounding box in bvh_node constructor.\n";

		return boxA.Min[axis] < boxB.Min[axis];
	}

	bool BVHNode::BoxXCompare(const Ref<Hittable> a, const Ref<Hittable> b)
	{
		return BoxCompare(a, b, 0);
	}

	bool BVHNode::BoxYCompare(const Ref<Hittable> a, const Ref<Hittable> b)
	{
		return BoxCompare(a, b, 1);
	}

	bool BVHNode::BoxZCompare(const Ref<Hittable> a, const Ref<Hittable> b)
	{
		return BoxCompare(a, b, 2);
	}
}
