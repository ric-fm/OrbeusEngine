#pragma once

#include <string>

class Texture
{
private:
	unsigned int id;
	std::string filePath;
	std::string type;
	int width;
	int height;
	int channels;

public:
	Texture(const std::string& filePath, const std::string& type, bool flipVertical = true);
	~Texture();

	int getWidth() const { return width; }
	int getHeight() const { return height; }

	const std::string& getType() const { return type; }

	void bind(unsigned int slot = 0) const;
	void unbind() const;
};
