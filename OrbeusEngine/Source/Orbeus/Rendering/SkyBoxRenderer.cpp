#include "SkyBoxRenderer.h"

#include <GL/glew.h>

#include "Orbeus/Rendering/CubeMap.h"
#include "Orbeus/Rendering/Shader.h"

#include "Orbeus/Core/World.h"
#include "Orbeus/Core/Transform.h"
#include "Orbeus/Components/Camera.h"
#include "Orbeus/Rendering/Buffer.h"
#include "Orbeus/ResourceManagement/CubeMapLoader.h"
#include "Orbeus/ResourceManagement/ShaderLoader.h"


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

void SkyBoxRenderer::render(Camera* camera)
{
	// Change the depth function to allow the skybox pass the depth tests because we generate a cube
	// of 1.0f in the vertex positions, so we don't need to worry about the cubemap size
	glDepthFunc(GL_LEQUAL);

	shader->bind();
	Matrix4 viewMatrix = camera->getViewMatrix();

	// Remove the translation of the matrix so the skybox is centered on camera
	viewMatrix.buffer[3][0] = 0.0f;
	viewMatrix.buffer[3][1] = 0.0f;
	viewMatrix.buffer[3][2] = 0.0f;

	shader->SetMatrix("view", viewMatrix);
	shader->SetMatrix("projection", camera->getProjectionMatrix());
	shader->SetFloat3("fogColor", camera->getFogColor());

	cubeMap->bind();

	cubeMap->vertexArray->bind();
	cubeMap->vertexArray->draw();
	cubeMap->vertexArray->unbind();
	
	cubeMap->unbind();

	shader->unbind();

	// Reset the depth function to default
	glDepthFunc(GL_LESS);
}
