#pragma once

#include <string>

class Texture;

class TextureLoader
{
public:
	static Texture* loadTexture(const std::string& filePath, const std::string& type, float lodBIAS = -1.0f, bool flipVertical = true);
};
