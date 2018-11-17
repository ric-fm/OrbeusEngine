#include "MeshRenderer.h"

#include "Components/Mesh.h"
#include "Components/Camera.h"
#include "Core/World.h"
#include "Core/Transform.h"
#include "Math/Matrix4.h"

#include "Rendering/Mesh\MeshData.h"
#include "Rendering/Material.h"
#include "Rendering/VertexArray.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/Shader.h"

void MeshRenderer::render(Shader* shader)
{
	std::vector<Mesh*> meshes = World::getInstance().getComponents<Mesh>();

	if (meshes.size() > 0)
	{
		shader->bind();
		shader->SetMatrix("view", World::getInstance().getActiveCamera()->getViewMatrix());
		shader->SetMatrix("projection", World::getInstance().getActiveCamera()->getProjectionMatrix());

		for (unsigned int i = 0; i < meshes.size(); ++i)
		{
			Mesh* mesh = meshes[i];

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
		}

		shader->unbind();
	}
}
