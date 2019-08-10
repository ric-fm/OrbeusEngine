#pragma once

namespace ORB
{
	class VertexArray;
	class VertexBuffer;
}

class CubeMap
{
	friend class CubeMapLoader;
	friend class SkyBoxRenderer;
private:
	unsigned int ID;

	ORB::VertexArray* vertexArray;
	ORB::VertexBuffer* vertexBuffer;

public:
	CubeMap();
	~CubeMap();

	void bind() const;
	void unbind() const;

};
