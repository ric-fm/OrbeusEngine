#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "Orbeus/Rendering/Shader.h"

class ShaderLoader
{
public:
	static Shader* loadShader(const std::string& vertexFilePath, const std::string& fragmentFilePath);

private:
	static void registerAttributesAndUniforms(Shader* shader);
};
