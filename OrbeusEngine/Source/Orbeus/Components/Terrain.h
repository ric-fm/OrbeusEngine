#pragma once

#include "Orbeus/Core/GameComponent.h"

#include <string>

class TerrainMeshData;

class Terrain : public GameComponent
{
	friend class TerrainRenderer;
	friend class TerrainLoader;
private:

	TerrainMeshData* terrainMeshData;

public:
	Terrain(const std::string& backgroundTexture, const std::string& redTexture,
		const std::string& greenTexture, const std::string& blueTexture, const std::string& blendMap, const std::string& heightMap);
	~Terrain();

};
