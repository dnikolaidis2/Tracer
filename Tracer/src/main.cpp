#include <iostream>

#include <stb_image_write.h>

#include "glm/glm.hpp"

void write_color(uint8_t* buffer, glm::vec3 color)
{
    buffer[0] = static_cast<int>(255.999 * color.r);
    buffer[1] = static_cast<int>(255.999 * color.g);
    buffer[2] = static_cast<int>(255.999 * color.b);
}

int main()
{
	// Image
	const int imageWidth = 256;
	const int imageHeight = 256;
    const int channelCount = 3;

	// Render

    uint8_t* imageBuffer = new uint8_t[imageWidth * imageHeight * channelCount];

    int imageIndex = 0;
    for (int j = imageHeight - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < imageWidth; ++i) {
            glm::vec3 pixelColor(double(i) / (imageWidth - 1), double(j) / (imageHeight - 1), 0.25f);

            write_color(imageBuffer + imageIndex, pixelColor);
            imageIndex += 3;
        }
    }

    std::cerr << "\nDone.\n";

    stbi_write_jpg("output.jpeg", imageWidth, imageHeight, channelCount, imageBuffer, 100);

	return 0;
}
