#pragma once

#include "Core/GameComponent.h"

#include <string>


class MeshData;

class Mesh : public GameComponent
{
private:
	MeshData* meshData;

	std::string meshFilePath;

public:
	Mesh(const std::string& meshFilePath);
	~Mesh();

	MeshData* getMeshData() const { return meshData; }

	void setMesh(const std::string& newMeshFilePath);
	std::string getMeshFilePath() const { return meshFilePath; }

};
