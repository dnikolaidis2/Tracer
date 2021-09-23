#include <iostream>

#include <stb_image_write.h>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

#include "Base.h"
#include "Ray.h"
#include "Hittable.h"
#include "HittableList.h"
#include "Sphere.h"
#include "Utils.h"
#include "Camera.h"

namespace TC {

    glm::dvec3 RayColor(const Ray& r, const Hittable& world)
    {
        HitRecord record;
		if (world.Hit(r, 0, INFINITY, record))
		{
            return 0.5 * glm::dvec3(record.Normal.x + 1, record.Normal.y + 1, record.Normal.z + 1);
		}

        glm::dvec3 directionNorm = glm::normalize(r.Direction);
        auto t = 0.5 * (directionNorm.y + 1.0);
        return (1.0 - t) * glm::dvec3(1.0, 1.0, 1.0) + t * glm::dvec3(0.5, 0.7, 1.0);
    }

    int EntryPoint()
    {
        // Image
        const auto aspectRatio = 16.0f / 9.0f;
        const int imageWidth = 720;
        const int imageHeight = static_cast<int>(imageWidth / aspectRatio);
        const int channelCount = 3;
        const int samplesPerPixel = 100;

        // World

        HittableList world;
        world.Add(CreateRef<Sphere>(glm::dvec3(0.0, 0.0, -1.0), 0.5));
        world.Add(CreateRef<Sphere>(glm::dvec3(0.0, -100.5, -1.0), 100));

        // Camera

        Camera camera;

        // Render

        uint8_t* imageBuffer = new uint8_t[imageWidth * imageHeight * channelCount];

        int imageIndex = 0;
        for (int j = imageHeight - 1; j >= 0; --j) {
            std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
            for (int i = 0; i < imageWidth; ++i) {
                glm::dvec3 pixelColor(0.0, 0.0, 0.0);
                for (int s = 0; s < samplesPerPixel; ++s)
                {
                    auto u = (i + RandomDouble()) / (imageWidth - 1);
                    auto v = (j + RandomDouble()) / (imageHeight - 1);

                    Ray r = camera.GetRay(u, v);
                    pixelColor += RayColor(r, world);
                }

                WriteColor(imageBuffer + imageIndex, pixelColor, samplesPerPixel);
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