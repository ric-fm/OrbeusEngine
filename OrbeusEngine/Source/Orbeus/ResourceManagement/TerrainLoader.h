#pragma once

#include <string>

#define HEIGHTMAP_MAX_PIXEL_COLOR 256.0f * 256.0f * 256.0f

class TerrainMeshData;

class TerrainLoader
{
public:
	static TerrainMeshData* generateTerrain(const std::string& backgroundTexture, const std::string& redTexture, 
		const std::string& greenTexture, const std::string& blueTexture, const std::string& blendMap, const std::string& heightMap,
		float maxHeight = 40.0f, float size = 800.0f);
};
