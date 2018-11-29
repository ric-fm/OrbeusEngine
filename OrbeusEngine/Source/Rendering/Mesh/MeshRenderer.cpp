#include "MeshRenderer.h"

#include "Components/Mesh.h"
#include "Components/Camera.h"
#include "Core/World.h"
#include "Core/Engine.h"
#include "Core/Transform.h"
#include "Math/Matrix4.h"

#include "Rendering/Mesh\MeshData.h"
#include "Rendering/Material.h"
#include "Rendering/VertexArray.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/RenderingEngine.h"
#include "Rendering/Shader.h"
#include "Components/Light.h"
#include "ResourceManagement/ShaderLoader.h"

MeshRenderer::MeshRenderer()
{
	shader = ShaderLoader::loadShader("Resources/Shaders/Mesh-vs.shader", "Resources/Shaders/Mesh-fs.shader");
}

MeshRenderer::~MeshRenderer()
{
	if (shader != nullptr)
	{
		delete shader;
	}
}

void MeshRenderer::render(Camera* camera)
{
	std::vector<Mesh*> meshes = World::getInstance().getComponents<Mesh>();
	std::vector<DirectionalLight*> directionalLights = World::getInstance().getComponents<DirectionalLight>();
	std::vector<PointLight*> pointLights = World::getInstance().getComponents<PointLight>();
	std::vector<SpotLight*> spotLights = World::getInstance().getComponents<SpotLight>();

	if (meshes.size() > 0)
	{
		shader->bind();
		shader->SetMatrix("view", camera->getViewMatrix());
		shader->SetMatrix("projection", camera->getProjectionMatrix());
		shader->SetFloat3("viewPos", camera->getTransform()->getPosition());
		shader->SetFloat3("ambientLight", camera->getAmbientLight());
		shader->SetFloat4("clipPlane", camera->getClipPlane());

		for (unsigned int i = 0; i < directionalLights.size() && i < 4; ++i)
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

		for (unsigned int i = 0; i < meshes.size(); ++i)
		{
			Mesh* mesh = meshes[i];

			if (mesh->getHasTransparency())
			{
				glDisable(GL_CULL_FACE);
			}

			shader->SetMatrix("model", mesh->getTransform()->getMatrix());

			MeshData* meshData = mesh->getMeshData();

			if (meshData->materials.size() > 0)
			{
				shader->SetBool("material.useDiffuseTexture", meshData->materials[0].useDiffuseTexture);
				shader->SetBool("material.useSpecularTexture", meshData->materials[0].useSpecularTexture);

				if (meshData->materials[0].diffuseTexture != nullptr && meshData->materials[0].useDiffuseTexture)
				{
					shader->SetInt("material.texture_diffuse", 0);
					meshData->materials[0].diffuseTexture->bind(0);
				}
				else
				{
					shader->SetFloat3("material.diffuse", meshData->materials[0].diffuse);
				}

				if (meshData->materials[0].specularTexture != nullptr && meshData->materials[0].useSpecularTexture)
				{
					shader->SetInt("material.texture_specular", 1);
					meshData->materials[0].specularTexture->bind(1);
				}
				else
				{
					shader->SetFloat3("material.specular", meshData->materials[0].specular);
				}


				shader->SetFloat("material.specularIntensity", meshData->materials[0].specularIntensity);
				shader->SetFloat("material.specularPower", meshData->materials[0].specularPower);
			}
			glActiveTexture(0);

			meshData->vertexArray->bind();
			meshData->vertexArray->draw(shader);
			meshData->vertexArray->unbind();

			if (mesh->getHasTransparency())
			{
				glEnable(GL_CULL_FACE);
				glCullFace(GL_BACK);
			}
		}

		shader->unbind();
	}
}
