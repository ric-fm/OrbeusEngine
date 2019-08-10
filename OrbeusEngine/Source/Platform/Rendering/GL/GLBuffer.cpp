#include "GLBuffer.h"

#include <GL/glew.h>

namespace ORB
{
	GLVertexBuffer::GLVertexBuffer(const void* data, unsigned int size, unsigned int count)
		: ID(0), count(count)
	{
		glGenBuffers(1, &ID);
		glBindBuffer(GL_ARRAY_BUFFER, ID);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	GLVertexBuffer::~GLVertexBuffer()
	{
		glDeleteBuffers(1, &ID);
	}

	void GLVertexBuffer::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, ID);
	}

	void GLVertexBuffer::unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}


	GLIndexBuffer::GLIndexBuffer(const unsigned int* data, unsigned int count)
		: count(count)
	{
		glGenBuffers(1, &ID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
	}

	GLIndexBuffer::~GLIndexBuffer()
	{
		glDeleteBuffers(1, &ID);
	}

	void GLIndexBuffer::bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	}

	void GLIndexBuffer::unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}


	static GLenum GetGLType(VertexBufferAttribute attribute)
	{
		switch (attribute.type)
		{
		case VertexBuferAttributeType::BOOL:
			return GL_BOOL;
			break;
		case VertexBuferAttributeType::INT:
		case VertexBuferAttributeType::INT2:
		case VertexBuferAttributeType::INT3:
		case VertexBuferAttributeType::INT4:
			return GL_INT;
			break;
		case VertexBuferAttributeType::FLOAT:
		case VertexBuferAttributeType::FLOAT2:
		case VertexBuferAttributeType::FLOAT3:
		case VertexBuferAttributeType::FLOAT4:
		case VertexBuferAttributeType::MATRIX3:
		case VertexBuferAttributeType::MATRIX4:
			return GL_FLOAT;
			break;
		}
		assert(false); // UNKNONW OPENGLTYPE
		return 0;
	}

	GLVertexArray::GLVertexArray()
		: ID(0), vertexBuffers(), indexBuffer(nullptr)
	{
		glGenVertexArrays(1, &ID);
		glBindVertexArray(ID);
	}

	GLVertexArray::~GLVertexArray()
	{
		glDeleteBuffers(1, &ID);
	}

	void GLVertexArray::addVertexBuffer(VertexBuffer* vertexBuffer, VertexBufferLayout layout)
	{
		GLVertexBuffer* buffer = static_cast<GLVertexBuffer*>(vertexBuffer);

		bind();
		buffer->bind();

		std::vector<VertexBufferAttribute>& attributes = layout.getAttributes();
		unsigned int offset = 0;
		for (unsigned int i = 0; i < attributes.size(); ++i)
		{
			VertexBufferAttribute& attribute = attributes[i];

			glEnableVertexAttribArray(i);
			glVertexAttribPointer(
				i,
				attribute.count,
				GetGLType(attribute),
				attribute.normalized ? GL_TRUE : GL_FALSE,
				layout.getStride(),
				(void*)offset
			);

			offset += attribute.count * GetAttributeTypeSize(attribute.type);
		}
		vertexBuffersCount += buffer->getCount();
		vertexBuffers.push_back(buffer);
	}

	void GLVertexArray::setIndexBuffer(IndexBuffer* indexBuffer)
	{
		this->indexBuffer = static_cast<GLIndexBuffer*>(indexBuffer);
	}

	void GLVertexArray::bind() const
	{
		glBindVertexArray(ID);
	}

	void GLVertexArray::unbind() const
	{
		glBindVertexArray(0);
	}

	void GLVertexArray::draw() const
	{
		bind();
		if (indexBuffer != nullptr)
		{
			glDrawElements(GL_TRIANGLES, indexBuffer->getCount(), GL_UNSIGNED_INT, 0);
		}
		else
		{
			glDrawArrays(GL_TRIANGLES, 0, vertexBuffersCount);
		}
	}
}