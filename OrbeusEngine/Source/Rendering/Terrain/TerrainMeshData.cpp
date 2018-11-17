#include "TerrainMeshData.h"

TerrainMeshData::TerrainMeshData()
{
}

TerrainMeshData::~TerrainMeshData()
{
	if (vertexArray != nullptr)
	{
		delete vertexArray;
	}
	if (vertexBuffer != nullptr)
	{
		delete vertexBuffer;
	}
	if (indexBuffer != nullptr)
	{
		delete indexBuffer;
	}
	if (texture != nullptr)
	{
		delete texture;
	}
}
