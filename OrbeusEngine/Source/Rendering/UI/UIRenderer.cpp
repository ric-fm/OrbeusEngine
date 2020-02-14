#include "UIRenderer.h"

#include "ResourceManagement/ShaderLoader.h"
#include "Rendering/Shader.h"

#include "Rendering/VertexBuffer.h"
#include "Rendering/VertexArray.h"

#include "Rendering/UI/UIComponent.h"
#include "Math/Matrix4.h"

#include "Core/Engine.h"
#include "Window/Window.h"

UIRenderer::UIRenderer()
{
	//projection = Matrix4::Orthographic(0, 800, 0, 600, -1, 1);
	shader = ShaderLoader::loadShader("Resources/Shaders/UI/UIComponent-vs.shader", "Resources/Shaders/UI/UIComponent-fs.shader");

	float w = 1.0f;
	float vertices[] = {
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 0.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f
	};

	vBuffer = new VertexBuffer(vertices, sizeof(vertices), 6);
	VertexBufferLayout layout;
	layout.Push<float>(2);

	vArray = new VertexArray();
	vArray->AddBuffer(vBuffer, layout);
}

UIRenderer::~UIRenderer()
{
}

void UIRenderer::render()
{
	projection = Matrix4::Orthographic(0, Engine::getInstance().getWindow()->getWidth(), 0, Engine::getInstance().getWindow()->getHeight(), -1, 1);

	shader->bind();
	shader->SetMatrix("projection", projection);
	vArray->bind();

	for (int i = 0; i < components.size(); ++i)
	{
		shader->SetMatrix("model", components[i]->getTransform().getMatrix());
		shader->SetFloat4("color", components[i]->getColor());
		vArray->draw(shader);
	}
	vArray->unbind();
	shader->unbind();
}

void UIRenderer::addComponent(UIComponent* component)
{
	components.push_back(component);
}
