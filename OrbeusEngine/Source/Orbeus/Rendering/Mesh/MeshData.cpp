#include "MeshData.h"

#include "Orbeus/Rendering\VertexArray.h"
#include "Orbeus/Rendering\VertexBuffer.h"
#include "Orbeus/Rendering\IndexBuffer.h"


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
