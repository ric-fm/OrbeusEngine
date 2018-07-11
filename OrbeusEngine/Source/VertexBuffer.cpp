#include "VertexBuffer.h"

#include "Shader.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size, unsigned int count)
	: id(0), count(count)
{
	glGenBuffers(1, &id);
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &id);
}

void VertexBuffer::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VertexBuffer::unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::draw(Shader* shader)
{
	glDrawArrays(GL_TRIANGLES, 0, count);
}
