#include "ResourceManager.h"

#include "Rendering/Text/Font.h"
#include "FontLoader.h"


ResourceManager::ResourceManager()
{
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
