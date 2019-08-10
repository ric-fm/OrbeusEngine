#pragma once

#include "Orbeus/Core/GameComponent.h"

#include <string>


class MeshData;

class Mesh : public GameComponent
{
private:
	MeshData* meshData;

	std::string meshFilePath;

	bool transparency;

public:
	Mesh(const std::string& meshFilePath, bool transparency = false);
	~Mesh();

	MeshData* getMeshData() const { return meshData; }

	void setMesh(const std::string& newMeshFilePath);
	std::string getMeshFilePath() const { return meshFilePath; }

	bool getHasTransparency() const { return transparency; }

};
