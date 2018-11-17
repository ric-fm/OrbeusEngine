#pragma once

#include <string>

class Font;

class FontLoader
{
public:
	static Font* LoadFont(const std::string& fontImagePath, const std::string& fontInfoPath);
};
