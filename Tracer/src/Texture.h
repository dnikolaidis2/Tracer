#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include <stb_image.h>

namespace TC {

	class Texture
	{
	public:
		virtual glm::dvec3 Value(double u, double v, const glm::dvec3& point) const = 0;
	};

	class SolidTexture : public Texture
	{
	public:
		SolidTexture() = default;
		SolidTexture(glm::dvec3 color) : m_Color(color) {}

		SolidTexture(double r, double g, double b) : m_Color(r, g, b) {}

		virtual glm::dvec3 Value(double u, double v, const glm::dvec3& point) const override
		{
			return m_Color;
		}
	private:
		glm::dvec3 m_Color;
	};

	class CheckerTexture : public Texture
	{
	public:
		CheckerTexture() {}
		CheckerTexture(Ref<Texture> evenTexture, Ref<Texture> oddTexture) : m_OddTexture(oddTexture), m_EvenTexture(evenTexture) {}

		CheckerTexture(glm::dvec3 evenColor, glm::dvec3 oddColor) : m_OddTexture(CreateRef<SolidTexture>(oddColor)), m_EvenTexture(CreateRef<SolidTexture>(evenColor)) {}

		virtual glm::dvec3 Value(double u, double v, const glm::dvec3& point) const override
		{
			auto sines = sin(10 * point.x) * sin(10 * point.y) * sin(10 * point.z);
			if (sines < 0)
				return m_OddTexture->Value(u, v, point);
			else
				return m_EvenTexture->Value(u, v, point);
		}
	private:
		Ref<Texture> m_OddTexture;
		Ref<Texture> m_EvenTexture;
	};

	class NoiseTexture : public Texture
	{
	public:
		NoiseTexture() {}
		NoiseTexture(double scale) : m_Scale(scale) {}

		virtual glm::dvec3 Value(double u, double v, const glm::dvec3& point) const override
		{
			return glm::dvec3(1.0) * 0.5 * (1.0 + glm::perlin(point * m_Scale));
		}
	private:
		double m_Scale;
	};

	class ImageTexture : public Texture
	{
	public:
		ImageTexture() : m_Data(nullptr), m_Width(0), m_Height(0), m_BytesPerScanline(0) {}

		ImageTexture(std::string filename)
		{
			auto channelsPerPixel = c_BytesPerPixel;

			stbi_set_flip_vertically_on_load(true);
			m_Data = stbi_load(filename.c_str(), &m_Width, &m_Height, &channelsPerPixel, channelsPerPixel);

			if (!m_Data)
			{
				std::cerr << "ERROR: Could not load texture image file '" << filename << "'.\n";
				m_Width = m_Height = 0;
			}

			m_BytesPerScanline = c_BytesPerPixel * m_Width;
		}

		~ImageTexture()
		{
			stbi_image_free(m_Data);
		}

		virtual glm::dvec3 Value(double u, double v, const glm::dvec3& point) const override
		{
			// If we have no texture data, then return cyan as a debugging aid.
			if (m_Data == nullptr)
				return glm::dvec3(0.0, 1.0, 1.0);

			// Clamp input texture coordinates to [0,1] x [1,0]
			u = std::clamp(u, 0.0, 1.0);
			v = std::clamp(v, 0.0, 1.0);

			auto i = static_cast<int>(u * m_Width);
			auto j = static_cast<int>(v * m_Height);

			// Clamp integer mapping, since actual coordinates should be less than 1.0
			if (i >= m_Width)  i = m_Width - 1;
			if (j >= m_Height) j = m_Height - 1;

			const auto colorScale = 1.0 / 255.0;
			auto pixel = m_Data + j * m_BytesPerScanline + i * c_BytesPerPixel;

			return colorScale * glm::dvec3(pixel[0], pixel[1], pixel[2]);
		}
	public:
		const static int c_BytesPerPixel = 3;
	private:
		uint8_t* m_Data;
		int m_Width, m_Height;
		int m_BytesPerScanline;
	};
}
