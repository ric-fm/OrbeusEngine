#pragma once

#include "Components/Camera.h"
class FrameBuffer;
class Texture;

class CameraCapture : public Camera
{
private:
	FrameBuffer* frameBuffer;
	Texture* colorTexture;
	Texture* depthTexture;

	int width;
	int height;

public:
	CameraCapture(int width, int height);
	~CameraCapture();

	virtual void init() override;

	void capture();


	int getWidth() const { return width; }
	int getHeight() const { return height; }

	Texture* getColorTexture() const { return colorTexture; }
	Texture* getDepthTexture() const { return depthTexture; }
};
