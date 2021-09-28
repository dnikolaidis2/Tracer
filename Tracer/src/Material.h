#pragma once

#include <glm/gtc/random.hpp>
#include <glm/gtc/epsilon.hpp>

#include "Ray.h"
#include "Texture.h"

namespace TC {
	struct HitRecord;

	class Material
	{
	public:
		virtual bool Scatter(const Ray& rIn, const HitRecord& record, glm::dvec3& attenuation, Ray& scattered) const = 0;
		virtual glm::dvec3 Emitted(double u, double v, const glm::dvec3& point) const
		{
			return glm::dvec3(0.0);
		}
	};

	class Lambertian : public Material
	{
	public:
		Lambertian(const glm::dvec3& albedo) : Albedo(CreateRef<SolidColor>(albedo)) {}
		Lambertian(Ref<Texture> texture) : Albedo(texture) {}

		virtual bool Scatter(const Ray& rIn, const HitRecord& record, glm::dvec3& attenuation, Ray& scattered) const override
		{
			auto scatterDirection = record.Normal + glm::ballRand(1.0);

			// Catch degenerate scatter direction
			if (glm::all(glm::epsilonEqual(scatterDirection, glm::dvec3(0.0), epsilon)))
				scatterDirection = record.Normal;

			scattered = Ray(record.Point, scatterDirection, rIn.Time);
			attenuation = Albedo->Value(record.U, record.V, record.Point);
			return true;
		}

	public:
		Ref<Texture> Albedo;
	};

	class Metal : public Material
	{
	public:
		Metal(const glm::dvec3& albedo, double fuzziness) : Albedo(albedo), Fuzziness(fuzziness) {}

		virtual bool Scatter(const Ray& rIn, const HitRecord& record, glm::dvec3& attenuation, Ray& scattered) const override
		{
			glm::dvec3 reflected = glm::reflect(glm::normalize(rIn.Direction), record.Normal);
			scattered = Ray(record.Point, reflected + Fuzziness * glm::ballRand(1.0), rIn.Time);
			attenuation = Albedo;
			return (glm::dot(scattered.Direction, record.Normal) > 0);
		}
	public:
		glm::dvec3 Albedo;
		double Fuzziness;
	};

	class Dielectric : public Material
	{
	public:
		Dielectric(double refractionIndex) : RefractionIndex(refractionIndex) {}

		virtual bool Scatter(const Ray& rIn, const HitRecord& record, glm::dvec3& attenuation, Ray& scattered) const override
		{
			attenuation = glm::dvec3(1.0);
			double refractionRatio = record.FrontFace ? (1.0 / RefractionIndex) : RefractionIndex;

			glm::dvec3 unitDirection = glm::normalize(rIn.Direction);
			glm::dvec3 refracted = glm::refract(unitDirection, record.Normal, refractionRatio);
			double cosTheta = std::fmin(glm::dot(-unitDirection, record.Normal), 1.0);
			double sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);

			bool cannotRefract = (refractionRatio * sinTheta) > 1.0;
			glm::dvec3 direction;

			if (cannotRefract || (Reflectance(cosTheta, refractionRatio) > glm::linearRand(0.0, 1.0)))
				direction = glm::reflect(unitDirection, record.Normal);
			else
				direction = glm::refract(unitDirection, record.Normal, refractionRatio);

			scattered = Ray(record.Point, direction, rIn.Time);
			return true;
		}
	private:
		static double Reflectance(double cosine, double reflectionIndex)
		{
			// Schlick's approximation for reflectance
			auto r0 = (1 - reflectionIndex) / (1 + reflectionIndex);
			r0 = r0 * r0;
			return r0 + (1 - r0) * std::pow((1 - cosine), 5);
		}
	public:
		double RefractionIndex;
	};

	class DiffuseLight : public Material
	{
	public:
		DiffuseLight(Ref<Texture> texture) : Emission(texture) {}
		DiffuseLight(glm::dvec3 color) : Emission(CreateRef<SolidColor>(color)) {}

		virtual bool Scatter(const Ray& rIn, const HitRecord& record, glm::dvec3& attenuation, Ray& scattered) const override
		{
			return false;
		}

		virtual glm::dvec3 Emitted(double u, double v, const glm::dvec3& point) const override
		{
			return Emission->Value(u, v, point);
		}
	public:
		Ref<Texture> Emission;
	};

	class Isotropic : public Material
	{
	public:
		Isotropic(glm::dvec3 color) : Albedo(CreateRef<SolidColor>(color)) {}
		Isotropic(Ref<Texture> albedo) : Albedo(albedo) {}

		virtual bool Scatter(const Ray& rIn, const HitRecord& record, glm::dvec3& attenuation, Ray& scattered) const override
		{
			scattered = Ray(record.Point, glm::sphericalRand(1.0), rIn.Time);
			attenuation = Albedo->Value(record.U, record.V, record.Point);

			return true;
		}
	public:
		Ref<Texture> Albedo;
	};
}
