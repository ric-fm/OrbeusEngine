#pragma once

class VertexArray;
class VertexBuffer;
class IndexBuffer;
class Texture;

class TerrainMeshData
{
	friend class TerrainRenderer;
	friend class TerrainLoader;
private:
	VertexArray* vertexArray = nullptr;
	VertexBuffer* vertexBuffer = nullptr;
	IndexBuffer* indexBuffer = nullptr;

	Texture* backgroundTexture;
	Texture* redTexture;
	Texture* greenTexture;
	Texture* blueTexture;
	Texture* blendMap;

public:
	TerrainMeshData();
	~TerrainMeshData();

};
