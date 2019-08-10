#include "Quad.h"

#include "GL/glew.h"

#include "Orbeus/Rendering/VertexArray.h"
#include "Orbeus/Rendering/VertexBuffer.h"
#include "Orbeus/Rendering/Shader.h"


Quad::Quad(Shader* shader)
	: shader(shader)
{

	float size = 0.5f;
	float vertices[] =
	{
		-size, 0.0f, -size,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f,
		-size, 0.0f,  size,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f,
		 size, 0.0f, -size,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,

		 size, 0.0f, -size,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
		-size, 0.0f,  size,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f,
		 size, 0.0f,  size,		0.0f, 1.0f, 0.0f,	1.0f, 1.0f
	};

	vertexBuffer = new VertexBuffer(vertices, sizeof(vertices), 6);
	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);
	layout.Push<float>(2);

	vertexArray = new VertexArray();
	vertexArray->AddBuffer(vertexBuffer, layout);

}

Quad::~Quad()
{
	if (vertexArray != nullptr)
	{
		delete vertexArray;
	}
	if (vertexBuffer != nullptr)
	{
		delete vertexBuffer;
	}
}

void Quad::bind() const
{
	vertexArray->bind();
}

void Quad::unbind() const
{
	vertexArray->unbind();
}

void Quad::draw()
{
	vertexArray->draw(shader);
}
