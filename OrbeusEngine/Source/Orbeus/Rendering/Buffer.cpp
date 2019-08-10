#include "Buffer.h"

#include "Platform/Rendering/GL/GLBuffer.h"

namespace ORB
{
	VertexBuffer* VertexBuffer::Create(const void* data, unsigned int size, unsigned int count)
	{
		return new GLVertexBuffer(data, size, count);
	}

	IndexBuffer* IndexBuffer::Create(const unsigned int* data, unsigned int count)
	{
		return new GLIndexBuffer(data,  count);
	}

	VertexArray* VertexArray::Create()
	{
		return new GLVertexArray();
	}
}
