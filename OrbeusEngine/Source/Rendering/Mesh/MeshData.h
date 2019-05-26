#pragma once

#include <vector>

#include "Rendering\Material.h"

class VertexArray;
class VertexBuffer;
class IndexBuffer;
class Shader;

class MeshData
{
	friend class MeshLoader;
	friend class MeshRenderer;
private:
	VertexArray* vertexArray = nullptr;
	VertexBuffer* vertexBuffer = nullptr;
	IndexBuffer* indexBuffer = nullptr;

	std::vector<Material> materials;
public:
	MeshData();
	~MeshData();
};
