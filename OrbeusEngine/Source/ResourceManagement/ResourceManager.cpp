#include "ResourceManager.h"

#include "Rendering/Mesh/MeshData.h"
#include "MeshLoader.h"

#include "Rendering/Text/Font.h"
#include "FontLoader.h"


ResourceManager::ResourceManager()
{
}

MeshData* ResourceManager::getMesh(const std::string& meshPath)
{
	MeshData* meshData = meshes[meshPath];
	if (meshData == nullptr)
	{
		meshes[meshPath] = meshData = MeshLoader::loadMesh(meshPath);
	}
	return meshData;
}

Font* ResourceManager::getFont(const std::string& fontName)
{
	Font* font = fonts[fontName];
	if (font == nullptr)
	{
		std::string texturePath = Font::FONTS_PATH + fontName + ".png";
		std::string fntPath = Font::FONTS_PATH + fontName + ".fnt";

		fonts[fontName] = font = FontLoader::LoadFont(texturePath, fntPath);
	}
	return font;
}
