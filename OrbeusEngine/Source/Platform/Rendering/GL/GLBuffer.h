#pragma once

#include "Orbeus/Rendering/Buffer.h"

namespace ORB
{
	class GLVertexBuffer : public VertexBuffer
	{
	public:
		GLVertexBuffer(const void* data, unsigned int size, unsigned int count);
		virtual ~GLVertexBuffer() override;
		virtual void bind() const override;
		virtual void unbind() const override;

		unsigned int getCount() const { return count; }

	private:
		unsigned int ID;
		unsigned int count;
	};


	class GLIndexBuffer : public IndexBuffer
	{
	public:
		GLIndexBuffer(const unsigned int* data, unsigned int count);
		virtual ~GLIndexBuffer() override;

		virtual void bind() const override;
		virtual void unbind() const override;

		unsigned int getCount() const { return count; }

	private:
		unsigned int ID;
		unsigned int count;
	};

	class GLVertexArray : public VertexArray
	{
	public:
		GLVertexArray();
		~GLVertexArray();

		virtual void addVertexBuffer(VertexBuffer* vertexBuffer, VertexBufferLayout layout) override;
		virtual void setIndexBuffer(IndexBuffer* indexBuffer) override;
		virtual bool isIndexed() const override { return indexBuffer != nullptr; }

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual void draw() const override;

	private:
		unsigned int ID;
		unsigned int vertexBuffersCount;
		std::vector<GLVertexBuffer*> vertexBuffers;
		GLIndexBuffer* indexBuffer;
	};
}