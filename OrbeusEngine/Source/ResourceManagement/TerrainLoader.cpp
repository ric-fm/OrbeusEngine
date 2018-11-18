#include "TerrainLoader.h"

#include <vector>

#include "MeshLoader.h"
#include "Rendering\VertexArray.h"
#include "Rendering\VertexBuffer.h"
#include "Rendering\IndexBuffer.h"
#include "Rendering\Texture.h"
#include "Rendering\Terrain\TerrainMeshData.h"
#include "Utils\ImageBuffer.h"

float getHeight(int x, int z, float maxHeight, ImageBuffer* image)
{
	float result = (float)image->getPixelRGBValue(x, z);
	result -= (HEIGHTMAP_MAX_PIXEL_COLOR / 2.0f);
	result /= (HEIGHTMAP_MAX_PIXEL_COLOR / 2.0f);
	result *= maxHeight;

	return result;
}

Vector3 calculateNormal(int x, int z, float maxHeight, ImageBuffer* image)
{
	float hL = x > 0 ? getHeight(x - 1, z, maxHeight, image) : 0.0f;
	float hR = x < image->getWidth() - 1 ? getHeight(x + 1, z, maxHeight, image) : 0.0f;
	float hD = z > 0 ? getHeight(x, z - 1, maxHeight, image) : 0.0f;
	float hU = z < image->getHeight() - 1 ? getHeight(x, z + 1, maxHeight, image) : 0.0f;

	return Vector3(
		hL - hR,
		2.0f,
		hD - hU
	).getNormalized();
}

TerrainMeshData * TerrainLoader::generateTerrain(const std::string & backgroundTexture, const std::string & redTexture,
	const std::string & greenTexture, const std::string & blueTexture, const std::string & blendMap, const std::string& heightMap,
	float maxHeight, float size)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	// Offset to set the pivot of the Terrain in (0,0,0)
	float offset = size / 2.0F;


	ImageBuffer* image = new ImageBuffer(heightMap);
	int vertexCount = image->getWidth();

	// Num of quads in one row or column
	int quadCount = vertexCount - 1;
	float inverseQuadCount = 1.0f / (float)quadCount;
	
	float quadSize = size / ((float)vertexCount - 1);

	// Generate (quadCount*quadCount) vertices
	for (int z = 0; z < vertexCount; z++) {
		for (int x = 0; x < vertexCount; x++) {

			vertices.push_back(
				{
					Vector3(x * quadSize - offset, getHeight(x,z, maxHeight, image), z * quadSize - offset),
					calculateNormal(x, z, maxHeight, image),
					Vector2(x * inverseQuadCount, z * inverseQuadCount)
				}
			);
		}
	}

	// Indexing the vertices of the Terrain
	indices.reserve(vertexCount * vertexCount);

	for (int z = 0; z < quadCount; z++) {
		for (int x = 0; x < quadCount; x++) {
			int topLeftIndex = (z*vertexCount) + x;
			int topRightIndex = topLeftIndex + 1;
			int bottomLeftIndex = ((z + 1)*vertexCount) + x;
			int bottomRightIndex = bottomLeftIndex + 1;

			indices.push_back(topLeftIndex);
			indices.push_back(bottomLeftIndex);
			indices.push_back(topRightIndex);
			indices.push_back(topRightIndex);
			indices.push_back(bottomLeftIndex);
			indices.push_back(bottomRightIndex);

			Vertex v;
			v = vertices[topLeftIndex];
			v = vertices[bottomLeftIndex];
			v = vertices[topRightIndex];
			v = vertices[topRightIndex];
			v = vertices[bottomLeftIndex];
			v = vertices[bottomRightIndex];
			v = vertices[bottomRightIndex];
		}
	}

	TerrainMeshData* result = new TerrainMeshData();

	result->vertexArray = new VertexArray();
	result->vertexBuffer = new VertexBuffer(&vertices[0], vertices.size() * sizeof(Vertex), vertices.size());
	result->indexBuffer = new IndexBuffer(&indices[0], indices.size());
	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);
	layout.Push<float>(2);

	result->vertexArray->AddBuffer(result->vertexBuffer, layout);

	result->backgroundTexture = new Texture(backgroundTexture, "background_texture", -0.4f);
	result->redTexture = new Texture(redTexture, "red_texture", -0.4f);
	result->greenTexture = new Texture(greenTexture, "green_texture", -0.4f);
	result->blueTexture = new Texture(blueTexture, "blue_texture", -0.4f);
	result->blendMap = new Texture(blendMap, "blend_map");

	return result;
}
