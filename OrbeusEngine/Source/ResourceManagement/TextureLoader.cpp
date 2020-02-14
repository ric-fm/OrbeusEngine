#include "TextureLoader.h"

#include "stb_image/stb_image.h"
#include "GL/glew.h"

#include "Rendering/Texture.h"
#include "Utils/Log.h"


Texture* TextureLoader::loadTexture(const std::string & filePath,const std::string& type, float lodBIAS, bool flipVertical)
{
	Texture* result = new Texture();
	result->type = type;

	stbi_set_flip_vertically_on_load(flipVertical);

	unsigned char* data = stbi_load(filePath.c_str(), &result->width, &result->height, &result->channels, NULL);

	if (data)
	{
		glGenTextures(1, &result->id);
		glBindTexture(GL_TEXTURE_2D, result->id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Trilinear filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, lodBIAS);

		unsigned int format;
		switch (result->channels)
		{
		case 1: format = GL_RED; break;
		case 4: format = GL_RGBA; break;
		default: format = GL_RGB; break;
		}

		glTexImage2D
		(
			GL_TEXTURE_2D,
			0,					// Mipmap level
			format,				// Format the texture will be stored
			result->width,		// Width of the source image
			result->height,		// Height of the source image
			0,					// border
			format,				// Format of the source image
			GL_UNSIGNED_BYTE,	// Data type of the source image data
			data				// Image data
		);

		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		Log::error("TEXTURE. Failed to lad texture %s", filePath.c_str());
	}

	stbi_image_free(data);

	return result;
}
