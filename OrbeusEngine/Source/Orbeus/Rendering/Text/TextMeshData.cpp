#include "TextMeshData.h"

#include "Orbeus/Rendering/Buffer.h"

TextMeshData::TextMeshData()
{
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
