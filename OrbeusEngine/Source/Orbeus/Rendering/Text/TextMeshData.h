#pragma once

#include <vector>

class VertexArray;
class VertexBuffer;

class TextMeshData
{
	friend class Font;
	friend class TextRenderer;
private:
	VertexArray* vertexArray = nullptr;
	VertexBuffer* vertexBuffer = nullptr;

public:
	TextMeshData();
	~TextMeshData();

};
