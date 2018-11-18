#pragma once

#include <string>

class ImageBuffer
{
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
	int getPixelRGB(unsigned int i, unsigned int j) const;
};
