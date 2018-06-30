#pragma once

#include <string>

#include <GL/glew.h>

class Shader
{
private:
	unsigned int id;

	std::string readShaderFile(const std::string& filePath);
	unsigned int createShader(const std::string& vertexFilePath, const std::string& fragmentFilePath);

public:
	Shader(const std::string& vertexFilePath, const std::string& fragmentFilePath);
	~Shader();

	void bind();
	void unbind();
};