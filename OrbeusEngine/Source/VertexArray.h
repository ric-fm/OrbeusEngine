#pragma once

#include <vector>

#include <GL/glew.h>

class VertexBuffer;
class Shader;

struct VertexBufferAttribute
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferAttribute> attributes;
	unsigned int stride;

public:
	VertexBufferLayout()
		: stride(0)
	{
	}

	std::vector<VertexBufferAttribute>& getAttributes() { return attributes; }
	unsigned int getStride() { return stride; }

	template<typename T>
	void Push(unsigned int count)
	{
		static_assert(false);
	}

	template<>
	void Push<float>(unsigned int count)
	{
		attributes.push_back({ GL_FLOAT, count, GL_FALSE });
		stride += count * sizeof(GL_FLOAT);
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		attributes.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		stride += count * sizeof(GL_UNSIGNED_INT);
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		attributes.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		stride += count * sizeof(GL_UNSIGNED_BYTE);
	}
};

class VertexArray
{
private:
	unsigned int id;

	VertexBuffer* vertexBuffer;

public:
	VertexArray();
	~VertexArray();

	void AddBuffer(VertexBuffer* vertexBuffer, VertexBufferLayout layout);

	void bind();
	void unbind();

	void draw(Shader& shader);
};