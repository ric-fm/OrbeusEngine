#pragma once

#include <vector>

#include "Math/Matrix4.h"

class Shader;
class Camera;
class VertexBuffer;
class VertexArray;
class UIComponent;

class UIRenderer
{
private:
	Matrix4 projection;
	Shader* shader;
	VertexBuffer* vBuffer;
	VertexArray* vArray;

	std::vector<UIComponent*> components;
public:
	UIRenderer();
	~UIRenderer();
	void render();

	void addComponent(UIComponent* component);
};
