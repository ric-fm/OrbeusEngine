#include "WaterPlane.h"

#include "Orbeus/ResourceManagement/ShaderLoader.h"
#include "Orbeus/Rendering/Shader.h"
#include "Orbeus/Rendering/Quad.h"
#include "Orbeus/Rendering/Texture.h"

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
