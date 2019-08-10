#pragma once

#include <string>

class Shader;

class ShaderLoader
{
public:
	static Shader* loadShader(const std::string& vertexFilePath, const std::string& fragmentFilePath);
};
