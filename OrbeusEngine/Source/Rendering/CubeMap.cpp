#include "CubeMap.h"

#include <GL/glew.h>

#include "Rendering/VertexArray.h"
#include "Rendering/VertexBuffer.h"


CubeMap::CubeMap()
{
}

CubeMap::~CubeMap()
{
	glDeleteTextures(1, &ID);

	if (vertexArray != nullptr)
	{
		delete vertexArray;
	}
	if (vertexBuffer != nullptr)
	{
		delete vertexBuffer;
	}
}

void CubeMap::bind() const
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
}

void CubeMap::unbind() const
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
