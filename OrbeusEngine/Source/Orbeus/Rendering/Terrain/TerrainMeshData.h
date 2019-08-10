#pragma once

namespace ORB
{
	class VertexArray;
	class VertexBuffer;
	class IndexBuffer;
}
class Texture;

class TerrainMeshData
{
	friend class TerrainRenderer;
	friend class TerrainLoader;
private:
	ORB::VertexArray* vertexArray = nullptr;
	ORB::VertexBuffer* vertexBuffer = nullptr;
	ORB::IndexBuffer* indexBuffer = nullptr;

	Texture* backgroundTexture;
	Texture* redTexture;
	Texture* greenTexture;
	Texture* blueTexture;
	Texture* blendMap;

public:
	TerrainMeshData();
	~TerrainMeshData();

};
