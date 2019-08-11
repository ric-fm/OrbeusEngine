#pragma once

#include <string>
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

	enum class AttributeType
	{
		NONE = 0,
		BOOL,
		INT, INT2, INT3, INT4,
		FLOAT, FLOAT2, FLOAT3, FLOAT4,
		MATRIX3, MATRIX4
	};

	static int GetAttributeTypeSize(AttributeType type)
	{
		switch (type)
		{
		case AttributeType::BOOL:
			return 1;
			break;
		case AttributeType::INT:
			return 4;
			break;
		case AttributeType::INT2:
			return 4*2;
			break;
		case AttributeType::INT3:
			return 4*3;
			break;
		case AttributeType::INT4:
			return 4*4;
			break;
		case AttributeType::FLOAT:
			return 4;
			break;
		case AttributeType::FLOAT2:
			return 4*2;
			break;
		case AttributeType::FLOAT3:
			return 4*3;
			break;
		case AttributeType::FLOAT4:
			return 4*4;
			break;
		case AttributeType::MATRIX3:
			return 3*3*4;
			break;
		case AttributeType::MATRIX4:
			return 4*4*4;
			break;
		}
		assert(false); // UNKNOW VERTEX BUFFER ATTRIBUTE TYPE
		return 0;
	}
	static int GetAttributeTypeCount(AttributeType type)
	{
		switch (type)
		{
		case AttributeType::BOOL:
		case AttributeType::INT:
		case AttributeType::FLOAT:
			return 1;
			break;
		case AttributeType::INT2:
		case AttributeType::FLOAT2:
			return 2;
			break;
		case AttributeType::FLOAT3:
		case AttributeType::INT3:
			return 3;
			break;
		case AttributeType::INT4:
		case AttributeType::FLOAT4:
			return 4;
			break;
		case AttributeType::MATRIX3:
			return 3 * 3;
			break;
		case AttributeType::MATRIX4:
			return 4 * 4;
			break;
		}
		assert(false); // UNKNOW ATTRIBUTE TYPE
		return 0;
	}

	struct VertexBufferAttribute
	{
		AttributeType type;
		unsigned char normalized;

		VertexBufferAttribute(AttributeType type, bool normalized = false)
			: type(type), normalized(normalized)
		{
		}
		VertexBufferAttribute(const std::string&, AttributeType type, bool normalized = false)
			: type(type), normalized(normalized)
		{
		}
	};

	class VertexBufferLayout
	{
	public:
		VertexBufferLayout()
			: stride(0)
		{
		}

		VertexBufferLayout(const std::initializer_list<VertexBufferAttribute>& attributes)
			: stride(0), attributes(attributes)
		{
			for (unsigned int i = 0; i < this->attributes.size(); ++i)
			{
				VertexBufferAttribute& attribute = this->attributes[i];
				stride += GetAttributeTypeSize(attribute.type);
			}
		}

		const std::vector<VertexBufferAttribute>& getAttributes() const { return attributes; }
		unsigned int getStride() const { return stride; }

		void push(VertexBufferAttribute attribute)
		{
			attributes.push_back(attribute);
			stride += GetAttributeTypeSize(attribute.type);
		}

	private:
		std::vector<VertexBufferAttribute> attributes;
		unsigned int stride;
	};

	class VertexArray
	{
	public:
		virtual ~VertexArray() = default;

		virtual void addVertexBuffer(VertexBuffer* vertexBuffer, VertexBufferLayout layout) = 0;
		virtual void setIndexBuffer(IndexBuffer* indexBuffer) = 0;
		virtual bool isIndexed() const = 0;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual void draw() const = 0;

		static VertexArray* Create();
	};
}
