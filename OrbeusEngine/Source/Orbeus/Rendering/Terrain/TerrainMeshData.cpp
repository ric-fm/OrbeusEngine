#include "TerrainMeshData.h"

#include "Orbeus/Rendering/VertexArray.h"
#include "Orbeus/Rendering/VertexBuffer.h"
#include "Orbeus/Rendering/IndexBuffer.h"
#include "Orbeus/Rendering/Texture.h"


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
	if (backgroundTexture != nullptr)
	{
		delete backgroundTexture;
	}
	if (redTexture != nullptr)
	{
		delete redTexture;
	}
	if (greenTexture != nullptr)
	{
		delete greenTexture;
	}
	if (blueTexture != nullptr)
	{
		delete blueTexture;
	}
	if (blendMap != nullptr)
	{
		delete blendMap;
	}
}
