#include "Terrain.h"

#include "Rendering\Terrain\TerrainMeshData.h"
#include "ResourceManagement\TerrainLoader.h"


Terrain::Terrain(const std::string& backgroundTexture, const std::string& redTexture,
	const std::string& greenTexture, const std::string& blueTexture, const std::string& blendMap, const std::string& heightMap)
{
	terrainMeshData = TerrainLoader::generateTerrain(backgroundTexture, redTexture, greenTexture, blueTexture, blendMap, heightMap);
}

Terrain::~Terrain()
{
	if (terrainMeshData != nullptr)
	{
		delete terrainMeshData;
	}
}
