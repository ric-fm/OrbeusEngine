#include "WaterPlane.h"

#include "ResourceManagement/ShaderLoader.h"
#include "Rendering/Shader.h"
#include "Rendering/Quad.h"
#include "Rendering/Texture.h"

WaterPlane::WaterPlane()
{
	Shader* waterShader = ShaderLoader::loadShader("Resources/Shaders/Water-vs.shader", "Resources/Shaders/Water-fs.shader");
	quad = new Quad(waterShader);
}

WaterPlane::~WaterPlane()
{
	delete quad;
	delete reflectionTexture;
	delete refractionTexture;
	delete refractionDepthTexture;
	delete dudvMap;
	delete normalMap;
}
