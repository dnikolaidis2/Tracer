#pragma once

#include <glm/glm.hpp>

#include "Ray.h"

namespace TC {
	class Camera
	{
	public:
		Camera();
		Ray GetRay(double u, double v) const;
	private:
		glm::dvec3 m_Origin;
		glm::dvec3 m_LowerLeftCorner;
		glm::dvec3 m_Horizontal;
		glm::dvec3 m_Vertical;
	};
}
