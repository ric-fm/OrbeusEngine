#include "VertexArray.h"

#include "VertexBuffer.h"
#include "Shader.h"

VertexArray::VertexArray()
	: id(0)
{
	glGenVertexArrays(1, &id);
	glBindVertexArray(id);
}

VertexArray::~VertexArray()
{
	glDeleteBuffers(1, &id);
}

void VertexArray::AddBuffer(VertexBuffer* vertexBuffer, VertexBufferLayout layout)
{
	this->vertexBuffer = vertexBuffer;

	bind();
	vertexBuffer->bind();

	std::vector<VertexBufferAttribute>& attributes = layout.getAttributes();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < attributes.size(); ++i)
	{
		VertexBufferAttribute& attribute = attributes[i];

		glEnableVertexAttribArray(i);
		glVertexAttribPointer(
			i, 
			attribute.count, 
			attribute.type, 
			attribute.normalized, 
			layout.getStride(), 
			(void*)offset
		);

		offset += attribute.count * sizeof(attribute.type);
	}
}

void VertexArray::bind()
{
	glBindVertexArray(id);
}

void VertexArray::unbind()
{
	glBindVertexArray(0);
}

void VertexArray::draw(Shader* shader)
{
	bind();
	vertexBuffer->draw(shader);
}
