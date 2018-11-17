#include "TerrainRenderer.h"

#include <vector>
#include <GL/glew.h>


#include "Rendering/VertexArray.h"
#include "Rendering/IndexBuffer.h"
#include "Rendering/Shader.h"
#include "Rendering/Texture.h"
#include "Rendering/Terrain/TerrainMeshData.h"

#include "Core/Transform.h"
#include "Core/World.h"
#include "Components/Terrain.h"
#include "Components/Camera.h"

TerrainRenderer::TerrainRenderer()
{
	shader = new Shader("Resources/Shaders/Terrain-vs.shader", "Resources/Shaders/Terrain-fs.shader");
}

TerrainRenderer::~TerrainRenderer()
{
	if (shader != nullptr)
	{
		delete shader;
	}
}

void TerrainRenderer::render()
{
	std::vector<Terrain*> terrains = World::getInstance().getComponents<Terrain>();

	if (terrains.size() > 0)
	{
		shader->bind();
		shader->SetMatrix("view", World::getInstance().getActiveCamera()->getViewMatrix());
		shader->SetMatrix("projection", World::getInstance().getActiveCamera()->getProjectionMatrix());
		
		shader->SetInt("texture_diffuse", 0);

		for (unsigned int i = 0; i < terrains.size(); ++i)
		{
			Terrain* terrain = terrains[i];
			
			shader->SetMatrix("model", terrain->getTransform()->getMatrix());

			TerrainMeshData* terrainMeshData = terrain->terrainMeshData;

			terrainMeshData->vertexArray->bind();

			terrainMeshData->texture->bind();

			glDrawElements(GL_TRIANGLES, terrainMeshData->indexBuffer->getCount(), GL_UNSIGNED_INT, 0);

			terrainMeshData->texture->unbind();

			terrainMeshData->vertexArray->unbind();
		}
		glActiveTexture(0);
		shader->unbind();
	}
}
