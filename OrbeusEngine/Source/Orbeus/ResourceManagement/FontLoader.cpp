#include "FontLoader.h"

#include <unordered_map>
#include <fstream>
#include <sstream>

#include "Orbeus/Rendering/Text/Font.h"
#include "Orbeus/Rendering/Texture.h"
#include "Orbeus/ResourceManagement/ResourceManager.h"
#include "Orbeus/Utils/String.h"

static bool processLine(std::ifstream& file, std::unordered_map<std::string, std::string>& values)
{
	values.clear();

	std::string line;
	if (getline(file, line))
	{
		std::vector<std::string> splitedLine;
		splitString(line, ' ', splitedLine);

		std::vector<std::string> attribute;

		for (unsigned int i = 0; i < splitedLine.size(); ++i)
		{
			splitString(splitedLine[i], '=', attribute);

			if (attribute.size() == 2)
			{
				values[attribute[0]] = attribute[1];
			}
		}
		return true;
	}
	return false;
}

static void getValue(std::unordered_map<std::string, std::string>& values, const std::string& name, int& value)
{
	value = atoi(values[name].c_str());
}

static void getValue(std::unordered_map<std::string, std::string>& values, const std::string& name, float& value)
{
	value = (float)atof(values[name].c_str());
}

static void getValue(std::unordered_map<std::string, std::string>& values, const std::string& name, std::string& value)
{
	value = values[name];
}

static void getValue(std::unordered_map<std::string, std::string>& values, const std::string& name, std::vector<int>& value)
{
	std::string stringValue = values[name];

	std::vector<std::string> stringValues;
	splitString(stringValue, ',', value);
}

Font* FontLoader::LoadFont(const std::string& fontImagePath, const std::string& fontInfoPath)
{
	Font* result = nullptr;

	// Load Texture
	Texture* atlas = ResourceManager::getInstance().getTexture(fontImagePath, "atlas", -1.0f, false);

	if (atlas != nullptr)
	{
		result = new Font();
		result->textureAtlas = atlas;

		// Open Font Info file
		std::ifstream file;
		file.open(fontInfoPath.c_str());

		if (file.is_open())
		{
			std::unordered_map<std::string, std::string> values;

			// Load general Info
			processLine(file, values);
			processLine(file, values);

			getValue(values, "face", result->name);

			float imageWidth;
			getValue(values, "scaleW", imageWidth);

			// Load characters
			processLine(file, values);
			processLine(file, values);

			std::unordered_map<int, Character> characters;

			while (processLine(file, values))
			{
				Character c;

				getValue(values, "id", c.id);
				getValue(values, "x", c.position.x);
				getValue(values, "y", c.position.y);
				getValue(values, "width", c.size.x);
				getValue(values, "height", c.size.y);
				getValue(values, "xoffset", c.baseLineOffset.x);
				getValue(values, "yoffset", c.baseLineOffset.y);
				getValue(values, "xadvance", c.nextCharacterOffset);

				// Set character info in UV Space
				c.position /= imageWidth;
				c.size /= imageWidth;
				c.baseLineOffset /= imageWidth;
				c.nextCharacterOffset /= imageWidth;

				result->characters[c.id] = c;
			}
		}
	}

	return result;
}
