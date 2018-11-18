#include "ImageBuffer.h"

#include <assert.h>

#include "stb_image/stb_image.h"

ImageBuffer::ImageBuffer(const std::string& filePath)
	: filePath(filePath)
{
	data = stbi_load(filePath.c_str(), &width, &height, &channels, NULL);
}

ImageBuffer::~ImageBuffer()
{
	if (data != nullptr)
	{
		stbi_image_free(data);
		delete data;
	}
}

int ImageBuffer::getPixelRGBValue(unsigned int i, unsigned int j) const
{
	assert(i <= width && j <= height);

	unsigned int offset = (i * width + j) * channels;
	unsigned char r = data[offset];
	unsigned char g = data[offset + 1];
	unsigned char b = data[offset + 2];

	return 0xFFFF * r + 0xFF * g + b;
}

Vector3 ImageBuffer::getPixelRGB(unsigned int i, unsigned int j) const
{
	assert(i <= width && j <= height);

	unsigned int offset = (i * width + j) * channels;

	return Vector3(
		data[offset],
		data[offset + 1],
		data[offset + 2]
	);
}
