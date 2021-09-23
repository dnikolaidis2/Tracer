#include <iostream>

#include <stb_image_write.h>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

#include "Ray.h"

namespace TC {

    // t^2 * b * b + 2 * t * b * (A - C) + (A - C) * (A - C) - r^2 = 0
    double HitSphere(const glm::dvec3& center, double radius, const Ray& r)
    {
        glm::dvec3 oc = r.Origin - center;
        auto a = glm::length2(r.Direction);
        auto halfB = glm::dot(oc, r.Direction);
        auto c = glm::length2(oc) - radius * radius;
        auto discriminant = halfB * halfB - a * c;

        if (discriminant < 0)
        {
            return -1.0;
        }
        else
        {
            return (-halfB - sqrt(discriminant)) / a;
        }
    }

    glm::dvec3 RayColor(const Ray& r)
    {
        glm::dvec3 center(0.0, 0.0, -1.0);
        auto t = HitSphere(center, 0.5, r);
		if (t > 0.0)
		{
            glm::dvec3 Normal = glm::normalize(r.At(t) - center);
            return 0.5f * glm::vec3(Normal.x + 1, Normal.y + 1, Normal.z + 1);
		}

        glm::dvec3 directionNorm = glm::normalize(r.Direction);
        t = 0.5 * (directionNorm.y + 1.0);
        return (1.0 - t) * glm::dvec3(1.0, 1.0, 1.0) + t * glm::dvec3(0.5, 0.7, 1.0);
    }

    void WriteColor(uint8_t* buffer, glm::vec3 color)
    {
        buffer[0] = static_cast<int>(255.999 * color.r);
        buffer[1] = static_cast<int>(255.999 * color.g);
        buffer[2] = static_cast<int>(255.999 * color.b);
    }

    int EntryPoint()
    {
        // Image
        const auto aspectRatio = 16.0f / 9.0f;
        const int imageWidth = 400;
        const int imageHeight = static_cast<int>(imageWidth / aspectRatio);
        const int channelCount = 3;

        // Camera

        auto viewportHeight = 2.0;
        auto viewportWidth = aspectRatio * viewportHeight;
        auto focalLength = 1.0;

        auto origin = glm::dvec3(0.0);
        auto horizontal = glm::dvec3(viewportWidth, 0.0, 0.0);
        auto vertical = glm::dvec3(0.0, viewportHeight, 0.0);
        auto lowerLeftCorner = origin - horizontal / 2.0 - vertical / 2.0 - glm::dvec3(0.0, 0.0, focalLength);

        // Render

        uint8_t* imageBuffer = new uint8_t[imageWidth * imageHeight * channelCount];

        int imageIndex = 0;
        for (int j = imageHeight - 1; j >= 0; --j) {
            std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
            for (int i = 0; i < imageWidth; ++i) {
                auto u = double(i) / (imageWidth - 1);
                auto v = double(j) / (imageHeight - 1);
                Ray r(origin, lowerLeftCorner + u * horizontal + v * vertical - origin);

                glm::vec3 pixelColor = RayColor(r);
                WriteColor(imageBuffer + imageIndex, pixelColor);
                imageIndex += 3;
            }
        }

        std::cerr << "\nDone.\n";

        stbi_write_jpg("output.jpeg", imageWidth, imageHeight, channelCount, imageBuffer, 100);

        return 0;
    }
}

int main()
{
    return TC::EntryPoint();
}