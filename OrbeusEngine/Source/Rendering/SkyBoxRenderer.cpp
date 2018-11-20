#include "SkyBoxRenderer.h"

#include "CubeMap.h"
#include "Rendering/Shader.h"

#include "Core/World.h"
#include "Core/Transform.h"
#include "Components/Camera.h"
#include "Rendering/VertexArray.h"
#include "ResourceManagement/CubeMapLoader.h"
#include "ResourceManagement/ShaderLoader.h"


SkyBoxRenderer::SkyBoxRenderer()
{
	cubeMap = CubeMapLoader::generateCubeMap("Resources/Textures/SkyBox/test/");

	shader = ShaderLoader::loadShader("Resources/Shaders/SkyBox-vs.shader", "Resources/Shaders/SkyBox-fs.shader");
	shader->bind();
	shader->SetInt("skybox", 0);
}

SkyBoxRenderer::~SkyBoxRenderer()
{
	if (cubeMap != nullptr)
	{
		delete cubeMap;
	}
	if (shader != nullptr)
	{
		delete shader;
	}
}

void SkyBoxRenderer::render()
{
	// Change the depth function to allow the skybox pass the depth tests because we generate a cube
	// of 1.0f in the vertex positions, so we don't need to worry about the cubemap size
	glDepthFunc(GL_LEQUAL);

	shader->bind();
	Matrix4 viewMatrix = World::getInstance().getActiveCamera()->getViewMatrix();

	// Remove the translation of the matrix so the skybox is centered on camera
	viewMatrix.buffer[3][0] = 0.0f;
	viewMatrix.buffer[3][1] = 0.0f;
	viewMatrix.buffer[3][2] = 0.0f;

	shader->SetMatrix("view", viewMatrix);
	shader->SetMatrix("projection", World::getInstance().getActiveCamera()->getProjectionMatrix());
	shader->SetFloat3("fogColor", World::getInstance().getActiveCamera()->getFogColor());

	cubeMap->bind();

	cubeMap->vertexArray->bind();
	cubeMap->vertexArray->draw(shader);
	cubeMap->vertexArray->unbind();
	
	cubeMap->unbind();

	shader->unbind();

	// Reset the depth function to default
	glDepthFunc(GL_LESS);
}
