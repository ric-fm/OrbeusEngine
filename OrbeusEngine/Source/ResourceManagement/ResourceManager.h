#pragma once

#include <unordered_map>
#include <string>

class MeshData;
class Font;
class Texture;

class ResourceManager
{
public:
	static ResourceManager& getInstance()
	{
		static ResourceManager instance;

		return instance;
	}
private:
	ResourceManager();

public:
	ResourceManager(ResourceManager const&) = delete;
	void operator=(ResourceManager const&) = delete;


private:
	std::unordered_map<std::string, MeshData*> meshes;
	std::unordered_map<std::string, Texture*> textures;
	std::unordered_map<std::string, Font*> fonts;

public:
	MeshData* getMesh(const std::string& meshPath);
	Texture* getTexture(const std::string& texturePath, const std::string& type, float lodBIAS = -1.0f, bool flipVertical = true);
	Font* getFont(const std::string& fontName);
};
