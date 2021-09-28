#include <stb_image_write.h>
#include <glm/glm.hpp>
#include "glm/gtx/string_cast.hpp"
#include <glm/gtc/random.hpp>

#include "AARect.h"
#include "Base.h"
#include "Box.h"
#include "BVH.h"
#include "Ray.h"
#include "Hittable.h"
#include "HittableList.h"
#include "Sphere.h"
#include "Utils.h"
#include "Camera.h"
#include "Material.h"
#include "Window/Window.h"

namespace TC {

    // Image
    // static const auto aspectRatio = 16.0f / 9.0f;
    static const auto aspectRatio = 1.0;
    static const int imageHeight = 720;
    static const int imageWidth = static_cast<int>(imageHeight * aspectRatio);
    static const int channelCount = 3;
    static int samplesPerPixel = 200;
    static const int maxDepth = 50;
    static const int bufferSize = imageWidth * imageHeight * channelCount;
    static const int totalPixels = imageWidth * imageHeight;

    glm::dvec3 RayColor(const Ray& r, const glm::dvec3& backgroundColor, const Hittable& world, int depth)
    {
        HitRecord record;

        // If we've exceeded the ray bounce limit, no more light is gathered.
        if (depth <= 0)
            return glm::dvec3(0.0);

        if (!world.Hit(r, 0.001, infinity, record))
            return backgroundColor;
		
        Ray scattered;
        glm::dvec3 attenuation;
        glm::dvec3 emitted = record.Material->Emitted(record.U, record.V, record.Point);

        if (!record.Material->Scatter(r, record, attenuation, scattered))
            return emitted;

        return emitted + attenuation * RayColor(scattered, backgroundColor, world, depth - 1);
    }

    void RenderPartition(uint8_t* buffer, int width, int height, int posY, const Camera& camera, const Hittable& world, const glm::dvec3& backgroundColor, RendererStats* stats)
    {
        int imageIndex = 0;
        for (int j = 0; j < height; ++j) {
            for (int i = 0; i < width; ++i) {
                glm::dvec3 pixelColor(0.0);
                for (int s = 0; s < samplesPerPixel; ++s)
                {
                    auto u = (i + glm::linearRand(0.0, 1.0)) / (imageWidth - 1);
                    auto v = (posY + j + glm::linearRand(0.0, 1.0)) / (imageHeight - 1);

                    Ray r = camera.GetRay(u, v);
                    pixelColor += RayColor(r, backgroundColor, world, maxDepth);
                }

                WriteColor(buffer + imageIndex, pixelColor, samplesPerPixel);
                imageIndex += 3;
                stats->PixelsRendered += 1;
            }
        }
    }

    HittableList RandomScene() {
        HittableList world;

        auto checker = CreateRef<CheckerTexture>(glm::dvec3(0.2, 0.3, 0.1), glm::dvec3(0.9, 0.9, 0.9));
        world.Add(CreateRef<Sphere>(glm::dvec3(0, -1000, 0), 1000, CreateRef<Lambertian>(checker)));

        for (int a = -11; a < 11; a++) {
            for (int b = -11; b < 11; b++) {
                auto choose_mat = glm::linearRand(0.0, 1.0);
                glm::dvec3 center(a + 0.9 * glm::linearRand(0.0, 1.0), 0.2, b + 0.9 * glm::linearRand(0.0, 1.0));

                if ((center - glm::dvec3(4, 0.2, 0)).length() > 0.9) {
                    Ref<Material> sphere_material;

                    if (choose_mat < 0.8) {
                        // diffuse
                        auto albedo = glm::linearRand(glm::dvec3(0.0), glm::dvec3(1.0));
                        sphere_material = CreateRef<Lambertian>(albedo);
                        auto center2 = center + glm::dvec3(0, glm::linearRand(0.0, 0.5), 0);
                        world.Add(CreateRef<MovingSphere>(center, center2, 0.0, 1.0, 0.2, sphere_material));
                    }
                    else if (choose_mat < 0.95) {
                        // metal
                        auto albedo = glm::linearRand(glm::dvec3(0.5), glm::dvec3(1.0));
                        auto fuzz = glm::linearRand(0.0, 0.5);
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

    HittableList TwoSpheres() {
        HittableList objects;

        auto checker = CreateRef<CheckerTexture>(glm::dvec3(0.2, 0.3, 0.1), glm::dvec3(0.9, 0.9, 0.9));

        objects.Add(CreateRef<Sphere>(glm::dvec3(0, -10, 0), 10, CreateRef<Lambertian>(checker)));
        objects.Add(CreateRef<Sphere>(glm::dvec3(0, 10, 0), 10, CreateRef<Lambertian>(checker)));

        return objects;
    }

    HittableList TwoPerlinSpheres() {
        HittableList objects;

        auto prelinTexture = CreateRef<NoiseTexture>(4);

        objects.Add(CreateRef<Sphere>(glm::dvec3(0, -1000, 0), 1000, CreateRef<Lambertian>(prelinTexture)));
        objects.Add(CreateRef<Sphere>(glm::dvec3(0, 2, 0), 2, CreateRef<Lambertian>(prelinTexture)));

        return objects;
    }

    HittableList Earth() {
        HittableList objects;

        auto earthTexture = CreateRef<ImageTexture>(std::string("earthmap.jpg"));
        auto earthMaterial = CreateRef<Lambertian>(earthTexture);
        objects.Add(CreateRef<Sphere>(glm::dvec3(0, 0, 0), 2, earthMaterial));

        return objects;
    }

    HittableList SimpleLight()
    {
        HittableList objects;

        auto prelinTexture = CreateRef<NoiseTexture>(4);

        objects.Add(CreateRef<Sphere>(glm::dvec3(0, -1000, 0), 1000, CreateRef<Lambertian>(prelinTexture)));
        objects.Add(CreateRef<Sphere>(glm::dvec3(0, 2, 0), 2, CreateRef<Lambertian>(prelinTexture)));

        auto diffLight = CreateRef<DiffuseLight>(glm::dvec3(4.0));
        objects.Add(CreateRef<XYRect>(3, 5, 1, 3, -2, diffLight));

        return objects;
    }

    HittableList CornellBox() {
        HittableList objects;

        auto red = CreateRef<Lambertian>(glm::dvec3(.65, .05, .05));
        auto white = CreateRef<Lambertian>(glm::dvec3(.73, .73, .73));
        auto green = CreateRef<Lambertian>(glm::dvec3(.12, .45, .15));
        auto light = CreateRef<DiffuseLight>(glm::dvec3(15, 15, 15));

        objects.Add(CreateRef<YZRect>(0, 555, 0, 555, 555, green));
        objects.Add(CreateRef<YZRect>(0, 555, 0, 555, 0, red));
        objects.Add(CreateRef<XZRect>(213, 343, 227, 332, 554, light));
        objects.Add(CreateRef<XZRect>(0, 555, 0, 555, 0, white));
        objects.Add(CreateRef<XZRect>(0, 555, 0, 555, 555, white));
        objects.Add(CreateRef<XYRect>(0, 555, 0, 555, 555, white));

        objects.Add(make_shared<Box>(glm::dvec3(130, 0, 65), glm::dvec3(295, 165, 230), white));
        objects.Add(make_shared<Box>(glm::dvec3(265, 0, 295), glm::dvec3(430, 330, 460), white));

        return objects;
    }

    int EntryPoint()
    {
        // Window
        Window window(imageWidth, imageHeight, "Tracer");

    	// Threads
        const int threadCount = 16;
        auto threadPool = new std::thread[threadCount];

        // World

        HittableList world = RandomScene();
        glm::dvec3 lookFrom;
        glm::dvec3 lookAt;
        double vFOV = 40.0;
        auto aperture = 0.0;
        glm::dvec3 background(0.0, 0.0, 0.0);

        switch (0) {
	        case 1:
	            world = RandomScene();
                background = glm::dvec3(0.70, 0.80, 1.00);
	            lookFrom = glm::dvec3(13, 2, 3);
                lookAt = glm::dvec3(0, 0, 0);
	            vFOV = 20.0;
	            aperture = 0.1;
	            break;

	        case 2:
	            world = TwoSpheres();
                background = glm::dvec3(0.70, 0.80, 1.00);
                lookFrom = glm::dvec3(13, 2, 3);
                lookAt = glm::dvec3(0, 0, 0);
                vFOV = 20.0;
	            break;

            case 3:
                world = TwoPerlinSpheres();
                background = glm::dvec3(0.70, 0.80, 1.00);
                lookFrom = glm::dvec3(13, 2, 3);
                lookAt = glm::dvec3(0, 0, 0);
                vFOV = 20.0;
                break;

            case 4:
                world = Earth();
                background = glm::dvec3(0.70, 0.80, 1.00);
                lookFrom = glm::dvec3(13, 2, 3);
                lookAt = glm::dvec3(0, 0, 0);
                vFOV = 20.0;
                break;

            case 5:
                world = SimpleLight();
                samplesPerPixel = 400;
                background = glm::dvec3(0.0, 0.0, 0.0);
                lookFrom = glm::dvec3(26, 3, 6);
                lookAt = glm::dvec3(0, 2, 0);
                vFOV = 20.0;
                break;

            default:
            case 6:
                world = CornellBox();
                background = glm::dvec3(0, 0, 0);
                lookFrom = glm::dvec3(278, 278, -800);
                lookAt = glm::dvec3(278, 278, 0);
                vFOV = 40.0;
                break;
        }

        // Camera

        glm::dvec3 vup(0.0, 1.0, 0.0);
        auto distToFocus = 10.0;

        Camera camera(lookFrom, lookAt, vup, vFOV, aspectRatio, aperture, distToFocus, 0.0, 1.0);

        // Render
        RendererStats stats = {0};
        uint8_t* imageBuffer = new uint8_t[bufferSize];

        BVHNode test(world, 0.0, 1.0);

        for (int i = 0; i < threadCount; ++i)
        {
            int remainder = 0;
            if ((i == threadCount - 1) && (imageHeight % threadCount != 0))
            {
                remainder = imageHeight % threadCount;
            }

            threadPool[i] = std::thread(RenderPartition,
                imageBuffer + (i * (imageHeight / threadCount) * imageWidth * channelCount),
                imageWidth,
                (imageHeight / threadCount) + remainder,
                i * (imageHeight / threadCount),
                camera, test, background, &stats);
        }

        while (stats.PixelsRendered != totalPixels)
        {
            std::this_thread::sleep_for(std::chrono::seconds(5));
            std::cerr << "\rProgress: " << ((float)stats.PixelsRendered / (float)totalPixels) * 100.0f << "% " << std::flush;
        }

        for (int i = 0; i < threadCount; ++i)
        {
            threadPool[i].join();
        }

        std::cerr << "\nDone.\n";

        stbi_flip_vertically_on_write(true);
        stbi_write_jpg("output.jpeg", imageWidth, imageHeight, channelCount, imageBuffer, 100);

        window.Init();
        window.WriteToScreen(imageWidth, imageHeight, imageBuffer);

        while (!window.ShouldWindowClose())
        {
            window.Render();
        }

        return 0;
    }
}

int main()
{
    return TC::EntryPoint();
}