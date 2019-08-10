#pragma once

class VertexArray;
class VertexBuffer;

class CubeMap
{
	friend class CubeMapLoader;
	friend class SkyBoxRenderer;
private:
	unsigned int ID;

	VertexArray* vertexArray;
	VertexBuffer* vertexBuffer;

public:
	CubeMap();
	~CubeMap();

	void bind() const;
	void unbind() const;

};
