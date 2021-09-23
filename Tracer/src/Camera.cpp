#include "Camera.h"

namespace TC {
	Camera::Camera()
	{
		auto aspectRatio = 16.0f / 9.0f;
		auto viewportHeight = 2.0;
		auto viewportWidth = aspectRatio * viewportHeight;
		auto focalLength = 1.0;

		m_Origin = glm::dvec3(0, 0, 0);
		m_Horizontal = glm::dvec3(viewportWidth, 0.0, 0.0);
		m_Vertical = glm::dvec3(0.0, viewportHeight, 0.0);
		m_LowerLeftCorner = m_Origin - m_Horizontal / 2.0 - m_Vertical / 2.0 - glm::dvec3(0, 0, focalLength);
	}

	Ray Camera::GetRay(double u, double v) const
	{
		return Ray(m_Origin, m_LowerLeftCorner + u * m_Horizontal + v * m_Vertical - m_Origin);
	}
}
