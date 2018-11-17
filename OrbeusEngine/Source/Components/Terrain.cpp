#include "Terrain.h"

#include "Rendering\Terrain\TerrainMeshData.h"
#include "ResourceManagement\TerrainLoader.h"


Terrain::Terrain(const std::string& texturePath)
{
	terrainMeshData = TerrainLoader::generateTerrain(texturePath);
}

Terrain::~Terrain()
{
	if (terrainMeshData != nullptr)
	{
		delete terrainMeshData;
	}
}
