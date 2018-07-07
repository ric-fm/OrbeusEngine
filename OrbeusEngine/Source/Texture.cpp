#include "Texture.h"


#include "GL/glew.h"
#include "stb_image/stb_image.h"

#include "Log.h"

Texture::Texture(const std::string& filePath, const std::string& type)
	: id(0), filePath(filePath), type(type), width(0), height(0), channels(0)
{
	// Enable vertical flip to load the data correctly
	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &channels, NULL);

	if (data)
	{
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		unsigned int format;
		switch (channels)
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
			width,				// Width of the source image
			height,				// Height of the source image
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
}

Texture::~Texture()
{
	glDeleteTextures(1, &id);
}

void Texture::bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
