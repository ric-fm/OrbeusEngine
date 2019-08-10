#include "MeshData.h"

#include "Orbeus/Rendering/Buffer.h"

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
