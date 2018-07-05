#pragma once

#include "GameComponent.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "Vector3.h"
#include "Vector2.h"

#include "String.h"



struct OBJIndex
{
	unsigned int position;
	unsigned int texCoord;
	unsigned int normal;
};

struct OBJInfo
{
	std::string name;
	PathInfo pathInfo;
	std::vector<Vector3> positions;
	std::vector<Vector3> normals;
	std::vector<Vector2> texCoords;
	std::vector<OBJIndex> indices;
};

struct Vertex
{
	Vector3 position;
	Vector3 normal;
	Vector2 texCoord;
};

class VertexArray;
class VertexBuffer;

class Mesh : public GameComponent
{
private:
	OBJInfo ParseOBJFile(const std::string& filePath) const;
	OBJIndex ParseOBJIndex(const std::string& index) const;

	void CreateMesh(const OBJInfo& objInfo);

	VertexArray* vertexArray = nullptr;
	VertexBuffer* vertexBuffer = nullptr;

public:
	Mesh(const std::string& filePath);
	~Mesh();

	virtual void render(float deltaTime, Shader* shader) override;

};
