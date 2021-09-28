#include "Camera.h"

#include <random>
#include <glm/gtc/random.hpp>

#include "glm/gtx/norm.hpp"

namespace TC {

	Camera::Camera(glm::dvec3 lookFrom, glm::dvec3 lookAt, glm::dvec3 vup, double vFOV, double aspectRatio, double aperture, double focusDist, double startTime, double endTime)
		: m_StartTime(startTime), m_EndTime(endTime)
	{
		auto theta = glm::radians(vFOV);
		auto h = std::tan(theta / 2);
		auto viewportHeight = 2.0 * h;
		auto viewportWidth = aspectRatio * viewportHeight;

		m_W = glm::normalize(lookFrom - lookAt);
		m_U = glm::normalize(glm::cross(vup, m_W));
		m_V = glm::cross(m_W, m_U);

		m_Origin = lookFrom;
		m_Horizontal = focusDist * viewportWidth * m_U;
		m_Vertical = focusDist * viewportHeight * m_V;
		m_LowerLeftCorner = m_Origin - (m_Horizontal / 2.0) - (m_Vertical / 2.0) - (focusDist * m_W);

		m_LensRadius = aperture / 2.0;
	}

	Ray Camera::GetRay(double s, double t) const
	{
		glm::dvec2 rayOrigin = m_LensRadius * glm::diskRand(1.0);
		glm::dvec3 offset = m_U * rayOrigin.x + m_V * rayOrigin.y;
		return Ray(m_Origin + offset, m_LowerLeftCorner + s * m_Horizontal + t * m_Vertical - m_Origin - offset, glm::linearRand(m_StartTime, m_EndTime));
	}
}
