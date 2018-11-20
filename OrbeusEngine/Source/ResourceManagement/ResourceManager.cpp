#include "ResourceManager.h"

#include "Rendering/Mesh/MeshData.h"
#include "MeshLoader.h"

#include "Rendering/Texture.h"
#include "TextureLoader.h"

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

Texture* ResourceManager::getTexture(const std::string& texturePath, const std::string& type, float lodBIAS, bool flipVertical)
{
	Texture* texture = textures[texturePath];
	if (texture == nullptr)
	{
		textures[texturePath] = texture = TextureLoader::loadTexture(texturePath, type, lodBIAS, flipVertical);
	}
	return texture;
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
