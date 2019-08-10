#include "TextMeshData.h"

#include "Orbeus/Rendering\VertexArray.h"
#include "Orbeus/Rendering\VertexBuffer.h"

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
