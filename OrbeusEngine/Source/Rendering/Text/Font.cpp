#include "Font.h"

#include <vector>

#include "Rendering/Texture.h"
#include "TextMeshData.h"


const std::string Font::FONTS_PATH("Resources/Fonts/");

Font::Font()
{
}

Font::~Font()
{
	if (textureAtlas != nullptr)
	{
		delete textureAtlas;
	}
}

void addQuad(float minX, float minY, float maxX, float maxY, std::vector<float>& vertices)
{
	vertices.push_back(minX);
	vertices.push_back(minY);

	vertices.push_back(maxX);
	vertices.push_back(minY);

	vertices.push_back(minX);
	vertices.push_back(maxY);

	vertices.push_back(minX);
	vertices.push_back(maxY);

	vertices.push_back(maxX);
	vertices.push_back(minY);

	vertices.push_back(maxX);
	vertices.push_back(maxY);
}

TextMeshData* Font::loadText(const std::string& text, float fontSize)
{
	Vector2 cursor;

	std::vector<float> positions;
	std::vector<float> textureCoords;

	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character character = characters[*c];

		// Generate vertices positions for a quad
		float x = cursor.x + (character.baseLineOffset.x * fontSize);
		float y = cursor.y - (character.baseLineOffset.y * fontSize);
		float mX = x + (character.size.x * fontSize);
		float mY = y - (character.size.y * fontSize);
		addQuad(x, y, mX, mY, positions);

		// Generate vertices UVs for a quad
		addQuad(character.position.x, character.position.y, character.position.x + character.size.x, character.position.y + character.size.y, textureCoords);
		cursor.x += character.nextCharacterOffset  * fontSize;
	}

	return new TextMeshData(positions, textureCoords);
}
