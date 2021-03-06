#include "TextMeshData.h"

#include "Rendering\VertexArray.h"
#include "Rendering\VertexBuffer.h"

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
