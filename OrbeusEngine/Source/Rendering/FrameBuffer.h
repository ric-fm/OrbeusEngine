#pragma once


class Texture;

class FrameBuffer
{
private:
	unsigned int id;
public:
	FrameBuffer();
	~FrameBuffer();

	void bind(Texture* texture) const;
	void unbind() const;

	Texture* attachToColorTexture(int width, int height);
	Texture* attachToDepthTexture(int width, int height);

	// TODO: RenderBuffer Attachment
};
