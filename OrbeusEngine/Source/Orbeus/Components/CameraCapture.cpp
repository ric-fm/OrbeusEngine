#include "CameraCapture.h"

#include "Orbeus/Rendering/FrameBuffer.h"
#include "Orbeus/Rendering/Texture.h"


#include "Orbeus/Core/Engine.h"
#include "Orbeus/Rendering/RenderingEngine.h"


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
