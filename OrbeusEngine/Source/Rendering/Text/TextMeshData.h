#pragma once

#include <vector>

class VertexArray;
class VertexBuffer;
class Shader;

class TextMeshData
{
private:
	VertexArray* vertexArray = nullptr;
	VertexBuffer* vertexBuffer = nullptr;

public:
	TextMeshData(const std::vector<float>& positions, const std::vector<float>& textureCoords);
	~TextMeshData();

	void bind() const;
	void unbind() const;

	void draw(Shader* shader) const;

};
