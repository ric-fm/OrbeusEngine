#pragma once

#include <string>

#include "Math/Vector3.h"

class ImageBuffer
{
private:
	unsigned int id;
	std::string filePath;
	int width;
	int height;
	int channels;

	unsigned char* data;

public:
	ImageBuffer(const std::string& filePath);
	~ImageBuffer();

	int getWidth() const { return width; }
	int getHeight() const { return height; }
	int getChannels() const { return channels; }
	int getPixelRGBValue(unsigned int i, unsigned int j) const;
	Vector3 getPixelRGB(unsigned int i, unsigned int j) const;
};
