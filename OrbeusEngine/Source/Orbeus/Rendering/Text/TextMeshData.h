#pragma once

#include <vector>

namespace ORB
{
	class VertexArray;
	class VertexBuffer;
}

class TextMeshData
{
	friend class Font;
	friend class TextRenderer;
private:
	ORB::VertexArray* vertexArray = nullptr;
	ORB::VertexBuffer* vertexBuffer = nullptr;

public:
	TextMeshData();
	~TextMeshData();

};
