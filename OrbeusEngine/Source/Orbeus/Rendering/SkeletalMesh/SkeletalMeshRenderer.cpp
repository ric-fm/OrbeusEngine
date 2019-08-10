#include "SkeletalMeshRenderer.h"

#include <GL/glew.h>


#include "Orbeus/Components/SkeletalMesh.h"
#include "Orbeus/Components/Camera.h"
#include "Orbeus/Core/World.h"
#include "Orbeus/Core/Engine.h"
#include "Orbeus/Core/Transform.h"
#include "Orbeus/Math/Matrix4.h"

#include "Orbeus/Rendering/SkeletalMesh/Bone.h"
#include "Orbeus/Rendering/Mesh/MeshData.h"
#include "Orbeus/Rendering/Material.h"
#include "Orbeus/Rendering/Buffer.h"
#include "Orbeus/Rendering/RenderingEngine.h"
#include "Orbeus/Rendering/Shader.h"
#include "Orbeus/Components/Light.h"
#include "Orbeus/ResourceManagement/ShaderLoader.h"
#include "Orbeus/ResourceManagement/ResourceManager.h"

#include "Orbeus/Utils/Log.h"


void SkeletalMeshRenderer::recursiveDrawBones(Bone* currentBone)
{
	if (currentBone->name == "Bone.001_end")
		return;
	if (currentBone->name == "Bone")
		boneShader->SetFloat3("color", 1, 0, 0);
	else
		boneShader->SetFloat3("color", 0, 1, 0);


	boneShader->SetMatrix("boneTransform", currentBone->transformMatrix);
	boneMeshData->vertexArray->draw();

	for (Bone* childBone : currentBone->children)
	{
		recursiveDrawBones(childBone);
	}
}

void SkeletalMeshRenderer::recursiveGetBoneTransforms(Bone* currentBone, const Matrix4& parentTransform, std::vector<Matrix4>& transforms)
{

	Matrix4 boneTransform = parentTransform * currentBone->transformMatrix;
	//boneTransform = Matrix4();
	transforms.push_back(boneTransform);

	for (Bone* childBone : currentBone->children)
	{
		recursiveGetBoneTransforms(childBone, boneTransform, transforms);
	}
}

SkeletalMeshRenderer::SkeletalMeshRenderer()
{
	shader = ShaderLoader::loadShader("Resources/Shaders/SkeletalMesh-vs.shader", "Resources/Shaders/SkeletalMesh-fs.shader");
	boneShader = ShaderLoader::loadShader("Resources/Shaders/Bone-vs.shader", "Resources/Shaders/Bone-fs.shader");
	boneMeshData = ResourceManager::getInstance().getMesh("Resources/Models/Primitives/Bone.obj");
}

SkeletalMeshRenderer::~SkeletalMeshRenderer()
{
	if (shader != nullptr)
	{
		delete shader;
	}
	if (boneShader != nullptr)
	{
		delete boneShader;
	}
	if (boneMeshData != nullptr)
	{
		delete boneMeshData;
	}
}

void SkeletalMeshRenderer::render(Camera* camera)
{
	std::vector<SkeletalMesh*> meshes = World::getInstance().getComponents<SkeletalMesh>();
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
			SkeletalMesh* mesh = meshes[i];

			/*	if (mesh->getHasTransparency())
				{
					glDisable(GL_CULL_FACE);
				}*/

			shader->SetMatrix("model", mesh->getTransform()->getMatrix());

			MeshData* meshData = mesh->getMeshData();

#if 1 // Draw mesh
			if (drawMeshes)
			{
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

				/*meshData->vertexArray->bind();
				meshData->vertexArray->draw(shader);
				meshData->vertexArray->unbind();

				if (mesh->getHasTransparency())
				{
					glEnable(GL_CULL_FACE);
					glCullFace(GL_BACK);
				}*/


				if (mesh->getBoneData() != nullptr)
				{
					std::vector< Matrix4>& pose = mesh->currentPoseV;
					for (unsigned int i = 0; i < pose.size(); ++i)
					{
						shader->SetMatrix(("boneTransforms[" + std::to_string(i) + "]"), pose[i]);
					}
					std::vector<Matrix4> boneTransforms;
				}

				meshData->vertexArray->bind();
				meshData->vertexArray->draw();
				meshData->vertexArray->unbind();
			}
#endif

		}
		shader->unbind();

#if 1 // Draw bones
		//glDisable(GL_DEPTH_TEST);

		if (drawBones)
		{
			boneShader->bind();
			boneShader->SetMatrix("view", camera->getViewMatrix());
			boneShader->SetMatrix("projection", camera->getProjectionMatrix());
			boneShader->SetFloat3("viewPos", camera->getTransform()->getPosition());
			boneShader->SetFloat3("ambientLight", camera->getAmbientLight());
			boneShader->SetFloat4("clipPlane", camera->getClipPlane());

			for (unsigned int i = 0; i < directionalLights.size() && i < 4; ++i)
			{
				DirectionalLight* directionalLight = directionalLights[i];


				boneShader->SetFloat3("directionalLights[" + std::to_string(i) + "].base.color", directionalLight->getColor());
				boneShader->SetFloat("directionalLights[" + std::to_string(i) + "].base.intensity", directionalLight->getIntensity());
				boneShader->SetFloat3("directionalLights[" + std::to_string(i) + "].direction", directionalLight->getTransform()->getForwardVector());
			}

			for (unsigned int i = 0; i < pointLights.size() && i < 4; ++i)
			{
				PointLight* pointLight = pointLights[i];

				boneShader->SetFloat3("pointLights[" + std::to_string(i) + "].base.color", pointLight->getColor());
				boneShader->SetFloat("pointLights[" + std::to_string(i) + "].base.intensity", pointLight->getIntensity());
				boneShader->SetFloat3("pointLights[" + std::to_string(i) + "].position", pointLight->getTransform()->getPosition());
				boneShader->SetFloat("pointLights[" + std::to_string(i) + "].attenuation.constant", pointLight->getAttenuation().constant);
				boneShader->SetFloat("pointLights[" + std::to_string(i) + "].attenuation.linear", pointLight->getAttenuation().linear);
				boneShader->SetFloat("pointLights[" + std::to_string(i) + "].attenuation.exponential", pointLight->getAttenuation().exponential);
				boneShader->SetFloat("pointLights[" + std::to_string(i) + "].radius", pointLight->getRadius());
			}

			for (unsigned int i = 0; i < spotLights.size() && i < 4; ++i)
			{
				SpotLight* spotLight = spotLights[i];

				boneShader->SetFloat3("spotLights[" + std::to_string(i) + "].pointLight.base.color", spotLight->getColor());
				boneShader->SetFloat("spotLights[" + std::to_string(i) + "].pointLight.base.intensity", spotLight->getIntensity());
				boneShader->SetFloat3("spotLights[" + std::to_string(i) + "].pointLight.position", spotLight->getTransform()->getPosition());
				boneShader->SetFloat("spotLights[" + std::to_string(i) + "].pointLight.attenuation.constant", spotLight->getAttenuation().constant);
				boneShader->SetFloat("spotLights[" + std::to_string(i) + "].pointLight.attenuation.linear", spotLight->getAttenuation().linear);
				boneShader->SetFloat("spotLights[" + std::to_string(i) + "].pointLight.attenuation.exponential", spotLight->getAttenuation().exponential);
				boneShader->SetFloat("spotLights[" + std::to_string(i) + "].pointLight.radius", spotLight->getRadius());
				boneShader->SetFloat3("spotLights[" + std::to_string(i) + "].direction", spotLight->getTransform()->getForwardVector());
				boneShader->SetFloat("spotLights[" + std::to_string(i) + "].cutoff", spotLight->getCutOff());
			}

			for (unsigned int i = 0; i < meshes.size(); ++i)
			{
				SkeletalMesh* mesh = meshes[i];

				boneShader->SetMatrix("model", mesh->getTransform()->getMatrix());

				boneMeshData->vertexArray->bind();

				BoneData* boneData = mesh->getBoneData();
				if (boneData != nullptr)
				{
					//recursiveDrawBones(boneData->rootBone);
					std::vector< Matrix4>& pose = mesh->currentPoseV;

					for (unsigned int i = 0; i < pose.size(); ++i)
					{
						if (i != 2)
						{
							if (i == 0)
								boneShader->SetFloat3("color", 1, 0, 0);
							else
								boneShader->SetFloat3("color", 0, 1, 0);

							Matrix4 boneMat = pose[i] * boneData->bones[i]->transformMatrix;
							boneShader->SetMatrix("boneTransform", boneMat);
							boneMeshData->vertexArray->draw();
						}
					}
				}

				boneMeshData->vertexArray->unbind();
			}
			boneShader->unbind();
		}
		//glEnable(GL_DEPTH_TEST);
#endif
	}
}
