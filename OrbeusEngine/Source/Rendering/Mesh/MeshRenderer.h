#pragma once

#include "Components\Mesh.h"

class Shader;

class MeshRenderer
{
private:
	Shader* shader;
public:
	MeshRenderer();
	~MeshRenderer();
	void render();
};
