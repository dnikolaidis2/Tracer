#pragma once

#include <glm/glm.hpp>

#include "Ray.h"

namespace TC {
	class Camera
	{
	public:
		Camera(glm::dvec3 lookFrom, glm::dvec3 looAt, glm::dvec3 vup, double vFOV, double aspectRatio, double aperture, double focusDist);
		Ray GetRay(double s, double t) const;
	private:
		glm::dvec3 m_Origin;
		glm::dvec3 m_LowerLeftCorner;
		glm::dvec3 m_Horizontal;
		glm::dvec3 m_Vertical;
		glm::dvec3 m_U, m_V, m_W;
		double m_LensRadius;
	};
}
