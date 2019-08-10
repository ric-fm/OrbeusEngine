#include "Font.h"

#include <vector>
#include <assert.h> 

#include "Orbeus/Rendering/Texture.h"
#include "Orbeus/Rendering/Text/TextMeshData.h"

#include "Orbeus/Rendering/VertexArray.h"
#include "Orbeus/Rendering/VertexBuffer.h"


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
	TextMeshData* result = new TextMeshData();
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

	assert((positions.size() == textureCoords.size()) && (positions.size() % 2 == 0));

	std::vector<float> data;
	data.reserve(positions.size() * 2);

	for (unsigned int i = 0; i < positions.size(); i = i + 2)
	{
		data.push_back(positions[i]);
		data.push_back(positions[i + 1]);
		data.push_back(textureCoords[i]);
		data.push_back(textureCoords[i + 1]);
	}

	result->vertexBuffer = new VertexBuffer(&data[0], data.size() * sizeof(float), data.size());

	VertexBufferLayout layout;
	layout.Push<float>(2);
	layout.Push<float>(2);

	result->vertexArray = new VertexArray();
	result->vertexArray->AddBuffer(result->vertexBuffer, layout);

	return result;
}
