#include "CameraCapture.h"

#include "Rendering/FrameBuffer.h"
#include "Rendering/Texture.h"


#include "Core/Engine.h"
#include "Rendering/RenderingEngine.h"


CameraCapture::CameraCapture(int width, int height)
	: width(width), height(height)
{
}

CameraCapture::~CameraCapture()
{
	delete frameBuffer;
	delete colorTexture;
	delete depthTexture;
}

void CameraCapture::init()
{
	frameBuffer = new FrameBuffer();
	colorTexture = frameBuffer->attachToColorTexture(width, height);
	depthTexture = frameBuffer->attachToDepthTexture(width, height);
	frameBuffer->unbind();
}

void CameraCapture::capture()
{
	frameBuffer->bind(colorTexture);
	Engine::getInstance().getRenderingEngine()->renderScene(this);
	frameBuffer->unbind();
}
