#pragma once

namespace ORB
{
	class VertexBuffer;
	class VertexArray;
}
class Shader;

class Quad
{
	friend class WaterRenderer;
private:
	ORB::VertexArray* vertexArray;
	ORB::VertexBuffer* vertexBuffer;
	Shader* shader;

public:
	Quad(Shader* shader);
	~Quad();

	void bind() const;
	void unbind() const;
	void draw();
};
