#pragma once

#include <GL/glew.h>

class Shader;

class VertexBuffer
{
private:
	unsigned int id;
	unsigned int count;

public:
	VertexBuffer(const void* data, unsigned int size, unsigned int count);
	~VertexBuffer();

	void bind();

	void unbind();

	unsigned int getCount() const { return count; }

	void draw(Shader* shader);
};