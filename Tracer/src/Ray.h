#pragma once

#include "glm/glm.hpp"

namespace TC {
	class Ray
	{
	public:
		Ray() {}
		Ray(const glm::dvec3& origin, const glm::dvec3& direction, double time = 0.0)
			: Origin(origin), Direction(direction), Time(time)
		{}

		// glm::dvec3 GetOrigin() const { return m_Origin; }
		// glm::dvec3 GetDirection() const { return m_Direction; }


		glm::dvec3 At(double t) const
		{
			return Origin + t * Direction;
		}

	public:
		glm::dvec3 Origin;
		glm::dvec3 Direction;
		double Time;
	};
}
