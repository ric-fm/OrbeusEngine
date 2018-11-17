#pragma once

#include <string>

class TerrainMeshData;

class TerrainLoader
{
public:
	static TerrainMeshData* generateTerrain(const std::string& texturePath, int vertexCount = 128, float size = 800.0f);
};
