#include "MeshData.h"

#include "Rendering\VertexArray.h"
#include "Rendering\VertexBuffer.h"


MeshData::MeshData()
{
}

MeshData::~MeshData()
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
