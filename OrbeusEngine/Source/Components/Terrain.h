#pragma once

#include "Core/GameComponent.h"

#include <string>

class TerrainMeshData;

class Terrain : public GameComponent
{
	friend class TerrainRenderer;
	friend class TerrainLoader;
private:

	TerrainMeshData* terrainMeshData;

public:
	Terrain(const std::string& texturePath);
	~Terrain();

};
