#include "FrameBuffer.h"

#include "GL/glew.h"

#include "Core/Engine.h"
#include "Window/Window.h"
#include "Rendering/Texture.h"

FrameBuffer::FrameBuffer()
{
	glGenFramebuffers(1, &id);
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &id);
}

void FrameBuffer::bind(Texture* texture) const
{
	glBindTexture(GL_TEXTURE_2D, 0); // Make sure the texture is not bound
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	glViewport(0, 0, texture->getWidth(), texture->getHeight());
	texture->bind();
}

void FrameBuffer::unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Engine::getInstance().getWindow()->getWidth(), Engine::getInstance().getWindow()->getHeight());
}

Texture* FrameBuffer::attachToColorTexture(int width, int height)
{
	Texture* result = new Texture();

	glGenTextures(1, &result->id);
	result->width = width;
	result->height = height;
	result->bind();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	result->unbind();

	glFramebufferTexture2D
	(
		GL_FRAMEBUFFER, // Target (draw, read or both)
		GL_COLOR_ATTACHMENT0,  // Attachment type
		GL_TEXTURE_2D, // Type of texture to attach
		result->id, // Texture's id to attach
		0 // Mipmap level
	);

	return result;
}

Texture* FrameBuffer::attachToDepthTexture(int width, int height)
{
	Texture* result = new Texture();

	glGenTextures(1, &result->id);
	result->width = width;
	result->height = height;
	result->bind();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	result->unbind();

	glFramebufferTexture2D
	(
		GL_FRAMEBUFFER, // Target (draw, read or both)
		GL_DEPTH_ATTACHMENT,  // Attachment type
		GL_TEXTURE_2D, // Type of texture to attach
		result->id, // Texture's id to attach
		0 // Mipmap level
	);

	return result;
}
