#include "CubeMapLoader.h"

#include <vector>

#include <GL/glew.h>
#include "stb_image/stb_image.h"

#include "Orbeus/Rendering/Buffer.h"
#include "Orbeus/Rendering/CubeMap.h"
#include "Orbeus/Utils/Log.h"

CubeMap* CubeMapLoader::generateCubeMap(const std::string& path, float size)
{
	CubeMap* result = new CubeMap();

	glGenTextures(1, &result->ID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, result->ID);

	// The order of the textures to assign to the cubemap_texture object.
	// The front/back is inverted because we considered the positive z forward
	static const std::vector<std::string> ORDER = { "right", "left", "top", "bottom", "back", "front" };

	for (unsigned int i = 0; i < ORDER.size(); ++i)
	{
		std::string texturePath = path + ORDER[i] + ".png";
		// Enable vertical flip to load the data correctly
		int width;
		int height;
		int channels;

		unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &channels, NULL);

		if (data)
		{
			unsigned int format;
			switch (channels)
			{
			case 1: format = GL_RED; break;
			case 4: format = GL_RGBA; break;
			default: format = GL_RGB; break;
			}

			glTexImage2D
			(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, // CubeMap texture target
				0,					// Mipmap level
				format,				// Format the texture will be stored
				width,				// Width of the source image
				height,				// Height of the source image
				0,					// border
				format,				// Format of the source image
				GL_UNSIGNED_BYTE,	// Data type of the source image data
				data				// Image data
			);

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

			stbi_image_free(data);
		}
		else
		{
			Log::error("CUBEMAP. Failed to lad texture %s", texturePath.c_str());
		}

	}

	float VERTICES[] = {
		-size,  size, -size,
		-size, -size, -size,
		 size, -size, -size,
		 size, -size, -size,
		 size,  size, -size,
		-size,  size, -size,

		-size, -size,  size,
		-size, -size, -size,
		-size,  size, -size,
		-size,  size, -size,
		-size,  size,  size,
		-size, -size,  size,

		 size, -size, -size,
		 size, -size,  size,
		 size,  size,  size,
		 size,  size,  size,
		 size,  size, -size,
		 size, -size, -size,

		-size, -size,  size,
		-size,  size,  size,
		 size,  size,  size,
		 size,  size,  size,
		 size, -size,  size,
		-size, -size,  size,

		-size,  size, -size,
		 size,  size, -size,
		 size,  size,  size,
		 size,  size,  size,
		-size,  size,  size,
		-size,  size, -size,

		-size, -size, -size,
		-size, -size,  size,
		 size, -size, -size,
		 size, -size, -size,
		-size, -size,  size,
		 size, -size,  size
	};

	result->vertexArray = ORB::VertexArray::Create();
	result->vertexBuffer = ORB::VertexBuffer::Create(VERTICES, sizeof(VERTICES), 6 * 6);
	ORB::VertexBufferLayout layout;
	layout.Push<float>(3);

	result->vertexArray->AddBuffer(result->vertexBuffer, layout);

	return result;
}
