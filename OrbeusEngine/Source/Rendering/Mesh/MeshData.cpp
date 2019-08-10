#include "MeshData.h"

#include "Rendering\VertexArray.h"
#include "Rendering\VertexBuffer.h"
#include "Rendering\IndexBuffer.h"


MeshData::MeshData()
{
}

MeshData::~MeshData()
{
	if (vertexArray)
		delete vertexArray;
	if (vertexBuffer)
		delete vertexBuffer;
	if (indexBuffer)
		delete indexBuffer;
}
