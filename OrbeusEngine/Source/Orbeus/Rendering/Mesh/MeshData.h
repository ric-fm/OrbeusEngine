#pragma once

#include <vector>

#include "Orbeus/Rendering/Material.h"

namespace ORB
{
	class VertexArray;
	class VertexBuffer;
	class IndexBuffer;
}
class Shader;

class MeshData
{
	friend class MeshLoader;
	friend class FBXLoader;
	friend class MeshRenderer;
	friend class SkeletalMeshRenderer;
private:
	ORB::VertexArray* vertexArray = nullptr;
	ORB::VertexBuffer* vertexBuffer = nullptr;
	ORB::IndexBuffer* indexBuffer = nullptr;

	std::vector<Material> materials;
public:
	MeshData();
	~MeshData();
};
