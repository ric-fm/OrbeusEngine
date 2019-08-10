#pragma once

#include <vector>
#include <assert.h>


namespace ORB
{
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void bind() const = 0;

		virtual void unbind() const = 0;

		static VertexBuffer* Create(const void* data, unsigned int size, unsigned int count);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		static IndexBuffer* Create(const unsigned int* data, unsigned int count);
	};

	enum VertexBuferAttributeType
	{
		NONE = 0,
		BOOL,
		INT, INT2, INT3, INT4,
		FLOAT, FLOAT2, FLOAT3, FLOAT4,
		MATRIX3, MATRIX4
	};

	static int GetAttributeTypeSize(VertexBuferAttributeType type)
	{
		switch (type)
		{
		case ORB::BOOL:
			return 1;
			break;
		case ORB::INT:
			return 4;
			break;
		case ORB::INT2:
			return 4*2;
			break;
		case ORB::INT3:
			return 4*3;
			break;
		case ORB::INT4:
			return 4*4;
			break;
		case ORB::FLOAT:
			return 4;
			break;
		case ORB::FLOAT2:
			return 4*2;
			break;
		case ORB::FLOAT3:
			return 4*3;
			break;
		case ORB::FLOAT4:
			return 4*4;
			break;
		case ORB::MATRIX3:
			return 3*3*4;
			break;
		case ORB::MATRIX4:
			return 4*4*4;
			break;
		}
		assert(false); // UNKNOW VERTEX BUFFER ATTRIBUTE TYPE
		return 0;
	}

	struct VertexBufferAttribute
	{
		VertexBuferAttributeType type;
		unsigned int count;
		unsigned char normalized;
	};


	//@TODO: Use initializer lists
	class VertexBufferLayout
	{
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
			attributes.push_back({ VertexBuferAttributeType::FLOAT, count, false });
			stride += count * GetAttributeTypeSize(VertexBuferAttributeType::FLOAT);
		}

		template<>
		void Push<unsigned int>(unsigned int count)
		{
			attributes.push_back({ VertexBuferAttributeType::INT, count, false });
			stride += count * sizeof(VertexBuferAttributeType::INT);
		}

	private:
		std::vector<VertexBufferAttribute> attributes;
		unsigned int stride;
	};

	class VertexArray
	{
	public:
		virtual ~VertexArray() = default;

		virtual void AddBuffer(VertexBuffer* vertexBuffer, VertexBufferLayout layout) = 0;
		virtual void setIndexBuffer(IndexBuffer* indexBuffer) = 0;
		virtual bool isIndexed() const = 0;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual void draw() const = 0;

		static VertexArray* Create();
	};
}
