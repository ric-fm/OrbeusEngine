#pragma once

#include <unordered_map>
#include <string>

class Font;

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
	std::unordered_map<std::string, Font*> fonts;

public:
	Font* getFont(const std::string& fontName);
};
