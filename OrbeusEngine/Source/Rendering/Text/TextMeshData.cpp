#include "TextMeshData.h"

#include <assert.h> 

#include "Rendering\VertexArray.h"
#include "Rendering\VertexBuffer.h"
#include "Rendering\Shader.h"

TextMeshData::TextMeshData(const std::vector<float>& positions, const std::vector<float>& textureCoords)
{
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

	vertexBuffer = new VertexBuffer(&data[0], data.size() * sizeof(float), data.size());

	VertexBufferLayout layout;
	layout.Push<float>(2);
	layout.Push<float>(2);

	vertexArray = new VertexArray();
	vertexArray->AddBuffer(vertexBuffer, layout);
}

TextMeshData::~TextMeshData()
{
	if (vertexArray == nullptr)
	{
		delete vertexArray;
	}

	if (vertexBuffer == nullptr)
	{
		delete vertexBuffer;
	}
}

void TextMeshData::bind() const
{
	vertexArray->bind();
}

void TextMeshData::unbind() const
{
	vertexArray->unbind();
}

void TextMeshData::draw(Shader* shader) const
{
	vertexArray->draw(shader);
}
