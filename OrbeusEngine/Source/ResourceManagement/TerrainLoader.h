#pragma once

#include <string>

class TerrainMeshData;

class TerrainLoader
{
public:
	static TerrainMeshData* generateTerrain(const std::string& backgroundTexture, const std::string& redTexture, 
		const std::string& greenTexture, const std::string& blueTexture, const std::string& blendMap,
		int vertexCount = 128, float size = 800.0f);
};
