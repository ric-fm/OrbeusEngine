#pragma once

#include <string>
#include <unordered_map>

#include "Orbeus/Math/Vector2.h"

class Texture;
class TextMeshData;

struct Character
{
	int id; // ASCII value
	Vector2 position; // Location of the top left corner (UV Space)
	Vector2 size; // Size (UV Space)
	Vector2 baseLineOffset; // Offset to align the character in texts (UV Space)
	float nextCharacterOffset; // Offset of the next character in texts (UV Space)
};


class Font
{
	friend class FontLoader;
private:
	std::string name;
	Texture* textureAtlas;

	std::unordered_map<int, Character> characters;

public:
	static const std::string FONTS_PATH;

	Font();
	~Font();
	
	std::string getName() const { return name; }
	const Texture* getTextureAtlas() const { return textureAtlas; }

	TextMeshData* loadText(const std::string& text, float fontSize = 1.0f);

};
