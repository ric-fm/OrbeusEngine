#include "TerrainRenderer.h"

#include <vector>
#include <GL/glew.h>


#include "Rendering/VertexArray.h"
#include "Rendering/IndexBuffer.h"
#include "Rendering/Shader.h"
#include "Rendering/Texture.h"
#include "Rendering/RenderingEngine.h"
#include "Rendering/Terrain/TerrainMeshData.h"

#include "Core/Transform.h"
#include "Core/World.h"
#include "Core/Engine.h"
#include "Components/Terrain.h"
#include "Components/Camera.h"
#include "Components/Light.h"
#include "ResourceManagement/ShaderLoader.h"


TerrainRenderer::TerrainRenderer()
{
	shader = ShaderLoader::loadShader("Resources/Shaders/Terrain-vs.shader", "Resources/Shaders/Terrain-fs.shader");
	shader->bind();
	shader->SetInt("background_texture", 0);
	shader->SetInt("red_texture", 1);
	shader->SetInt("green_texture", 2);
	shader->SetInt("blue_texture", 3);
	shader->SetInt("blend_map", 4);
	shader->unbind();
}

TerrainRenderer::~TerrainRenderer()
{
	if (shader != nullptr)
	{
		delete shader;
	}
}

void TerrainRenderer::render(Camera* camera)
{
	std::vector<Terrain*> terrains = World::getInstance().getComponents<Terrain>();
	std::vector<DirectionalLight*> directionalLights = World::getInstance().getComponents<DirectionalLight>();
	std::vector<PointLight*> pointLights = World::getInstance().getComponents<PointLight>();
	std::vector<SpotLight*> spotLights = World::getInstance().getComponents<SpotLight>();

	if (terrains.size() > 0)
	{
		shader->bind();
		shader->SetMatrix("view", camera->getViewMatrix());
		shader->SetMatrix("projection", camera->getProjectionMatrix());
		shader->SetFloat3("viewPos", camera->getTransform()->getPosition());
		shader->SetFloat3("fogColor", camera->getFogColor());
		shader->SetFloat3("ambientLight", camera->getAmbientLight());
		shader->SetFloat("fogDensity", camera->getFogDensity());
		shader->SetFloat4("clipPlane", camera->getClipPlane());

		for(unsigned int i = 0; i < directionalLights.size() && i < 4; ++i)
		{
			DirectionalLight* directionalLight = directionalLights[i];


			shader->SetFloat3("directionalLights[" + std::to_string(i) + "].base.color", directionalLight->getColor());
			shader->SetFloat("directionalLights[" + std::to_string(i) + "].base.intensity", directionalLight->getIntensity());
			shader->SetFloat3("directionalLights[" + std::to_string(i) + "].direction", directionalLight->getTransform()->getForwardVector());
		}

		for (unsigned int i = 0; i < pointLights.size() && i < 4; ++i)
		{
			PointLight* pointLight = pointLights[i];

			shader->SetFloat3("pointLights[" + std::to_string(i) + "].base.color", pointLight->getColor());
			shader->SetFloat("pointLights[" + std::to_string(i) + "].base.intensity", pointLight->getIntensity());
			shader->SetFloat3("pointLights[" + std::to_string(i) + "].position", pointLight->getTransform()->getPosition());
			shader->SetFloat("pointLights[" + std::to_string(i) + "].attenuation.constant", pointLight->getAttenuation().constant);
			shader->SetFloat("pointLights[" + std::to_string(i) + "].attenuation.linear", pointLight->getAttenuation().linear);
			shader->SetFloat("pointLights[" + std::to_string(i) + "].attenuation.exponential", pointLight->getAttenuation().exponential);
			shader->SetFloat("pointLights[" + std::to_string(i) + "].radius", pointLight->getRadius());
		}

		for (unsigned int i = 0; i < spotLights.size() && i < 4; ++i)
		{
			SpotLight* spotLight = spotLights[i];

			shader->SetFloat3("spotLights[" + std::to_string(i) + "].pointLight.base.color", spotLight->getColor());
			shader->SetFloat("spotLights[" + std::to_string(i) + "].pointLight.base.intensity", spotLight->getIntensity());
			shader->SetFloat3("spotLights[" + std::to_string(i) + "].pointLight.position", spotLight->getTransform()->getPosition());
			shader->SetFloat("spotLights[" + std::to_string(i) + "].pointLight.attenuation.constant", spotLight->getAttenuation().constant);
			shader->SetFloat("spotLights[" + std::to_string(i) + "].pointLight.attenuation.linear", spotLight->getAttenuation().linear);
			shader->SetFloat("spotLights[" + std::to_string(i) + "].pointLight.attenuation.exponential", spotLight->getAttenuation().exponential);
			shader->SetFloat("spotLights[" + std::to_string(i) + "].pointLight.radius", spotLight->getRadius());
			shader->SetFloat3("spotLights[" + std::to_string(i) + "].direction", spotLight->getTransform()->getForwardVector());
			shader->SetFloat("spotLights[" + std::to_string(i) + "].cutoff", spotLight->getCutOff());
		}

		for (unsigned int i = 0; i < terrains.size(); ++i)
		{
			Terrain* terrain = terrains[i];
			
			shader->SetMatrix("model", terrain->getTransform()->getMatrix());

			TerrainMeshData* terrainMeshData = terrain->terrainMeshData;

			terrainMeshData->vertexArray->bind();

			terrainMeshData->backgroundTexture->bind(0);
			terrainMeshData->redTexture->bind(1);
			terrainMeshData->greenTexture->bind(2);
			terrainMeshData->blueTexture->bind(3);
			terrainMeshData->blendMap->bind(4);

			glDrawElements(GL_TRIANGLES, terrainMeshData->indexBuffer->getCount(), GL_UNSIGNED_INT, 0);

			terrainMeshData->backgroundTexture->unbind();

			terrainMeshData->vertexArray->unbind();
		}
		glActiveTexture(0);
		shader->unbind();
	}
}
