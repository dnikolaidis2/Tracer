#include <iostream>

#include <stb_image_write.h>
#include <glm/glm.hpp>
#include "glm/gtx/string_cast.hpp"
#include <glm/gtc/random.hpp>

#include "Base.h"
#include "Ray.h"
#include "Hittable.h"
#include "HittableList.h"
#include "Sphere.h"
#include "Utils.h"
#include "Camera.h"
#include "Material.h"

namespace TC {

    glm::dvec3 RayColor(const Ray& r, const Hittable& world, int depth)
    {
        HitRecord record;

        // If we've exceeded the ray bounce limit, no more light is gathered.
        if (depth <= 0)
            return glm::dvec3(0.0);

		if (world.Hit(r, 0.001, infinity, record))
		{
            Ray scattered;
            glm::dvec3 attenuation;
            if (record.Material->Scatter(r, record, attenuation, scattered))
                return attenuation * RayColor(scattered, world, depth - 1);

            return glm::dvec3(0.0);
		}

        glm::dvec3 directionNorm = glm::normalize(r.Direction);
        auto t = 0.5 * (directionNorm.y + 1.0);
        return (1.0 - t) * glm::dvec3(1.0, 1.0, 1.0) + t * glm::dvec3(0.5, 0.7, 1.0);
    }

    HittableList RandomScene() {
        HittableList world;

        auto ground_material = CreateRef<Lambertian>(glm::dvec3(0.5, 0.5, 0.5));
        world.Add(CreateRef<Sphere>(glm::dvec3(0, -1000, 0), 1000, ground_material));

        for (int a = -11; a < 11; a++) {
            for (int b = -11; b < 11; b++) {
                auto choose_mat = RandomDouble();
                glm::dvec3 center(a + 0.9 * RandomDouble(), 0.2, b + 0.9 * RandomDouble());

                if ((center - glm::dvec3(4, 0.2, 0)).length() > 0.9) {
                    Ref<Material> sphere_material;

                    if (choose_mat < 0.8) {
                        // diffuse
                        auto albedo = glm::linearRand(glm::dvec3(0.0), glm::dvec3(1.0));
                        sphere_material = CreateRef<Lambertian>(albedo);
                        world.Add(CreateRef<Sphere>(center, 0.2, sphere_material));
                    }
                    else if (choose_mat < 0.95) {
                        // metal
                        auto albedo = glm::linearRand(glm::dvec3(0.5), glm::dvec3(1.0));
                        auto fuzz = RandomDouble(0, 0.5);
                        sphere_material = CreateRef<Metal>(albedo, fuzz);
                        world.Add(CreateRef<Sphere>(center, 0.2, sphere_material));
                    }
                    else {
                        // glass
                        sphere_material = CreateRef<Dielectric>(1.5);
                        world.Add(CreateRef<Sphere>(center, 0.2, sphere_material));
                    }
                }
            }
        }

        auto material1 = CreateRef<Dielectric>(1.5);
        world.Add(CreateRef<Sphere>(glm::dvec3(0, 1, 0), 1.0, material1));

        auto material2 = CreateRef<Lambertian>(glm::dvec3(0.4, 0.2, 0.1));
        world.Add(CreateRef<Sphere>(glm::dvec3(-4, 1, 0), 1.0, material2));

        auto material3 = CreateRef<Metal>(glm::dvec3(0.7, 0.6, 0.5), 0.0);
        world.Add(CreateRef<Sphere>(glm::dvec3(4, 1, 0), 1.0, material3));

        return world;
    }

    int EntryPoint()
    {
        // Image
        const auto aspectRatio = 16.0f / 9.0f;
        const int imageWidth = 1920;
        const int imageHeight = static_cast<int>(imageWidth / aspectRatio);
        const int channelCount = 3;
        const int samplesPerPixel = 500;
        const int maxDepth = 50;

        // World

        HittableList world = RandomScene();
        /*HittableList world;

        auto materialGround = CreateRef<Lambertian>(glm::dvec3(0.8, 0.8, 0.0));
        auto materialCenter = CreateRef<Lambertian>(glm::dvec3(0.1, 0.2, 0.5));
        auto materialLeft = CreateRef<Dielectric>(1.5);
        auto materialRight = CreateRef<Metal>(glm::dvec3(0.8, 0.6, 0.2), 1.0);
        
        world.Add(CreateRef<Sphere>(glm::dvec3(0.0, -100.5, -1.0), 100.0, materialGround));
        world.Add(CreateRef<Sphere>(glm::dvec3(0.0, 0.0, -1.0), 0.5, materialCenter));
        world.Add(CreateRef<Sphere>(glm::dvec3(-1.0, 0.0, -1.0), 0.5, materialLeft));
        world.Add(CreateRef<Sphere>(glm::dvec3(-1.0, 0.0, -1.0), -0.4, materialLeft));
        world.Add(CreateRef<Sphere>(glm::dvec3(1.0, 0.0, -1.0), 0.5, materialRight));*/

        // Camera

        glm::dvec3 lookFrom(13.0, 2.0, 3.0);
        glm::dvec3 lookAt(0.0, 0.0, 0.0);
        glm::dvec3 vup(0.0, 1.0, 0.0);
        auto distToFocus = 10;
        auto aperture = 0.1;

        Camera camera(lookFrom, lookAt, vup, 20.0, aspectRatio, aperture, distToFocus);

        // Render

        uint8_t* imageBuffer = new uint8_t[imageWidth * imageHeight * channelCount];

        int imageIndex = 0;
        for (int j = imageHeight - 1; j >= 0; --j) {
            std::cerr << "\rProgress: " << (1 - ((float)j / (float)imageHeight))*100.0f << "% " << std::flush;
            for (int i = 0; i < imageWidth; ++i) {
                glm::dvec3 pixelColor(0.0, 0.0, 0.0);
                for (int s = 0; s < samplesPerPixel; ++s)
                {
                    auto u = (i + RandomDouble()) / (imageWidth - 1);
                    auto v = (j + RandomDouble()) / (imageHeight - 1);

                    Ray r = camera.GetRay(u, v);
                    pixelColor += RayColor(r, world, maxDepth);
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