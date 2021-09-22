#pragma once
#include "glm/glm.hpp"

namespace TC {
	class Ray
	{
	public:
		Ray() {}
		Ray(const glm::vec3& origin, const glm::vec3& direction)
			: m_Origin(origin), m_Direction(direction)
		{}

		glm::vec3 GetOrigin() const { return m_Origin; }
		glm::vec3 GetDirection() const { return m_Direction; }


		glm::vec3 At(float t) const
		{
			return m_Origin + t * m_Direction;
		}

	public:
		glm::vec3 m_Origin;
		glm::vec3 m_Direction;
	};
}
