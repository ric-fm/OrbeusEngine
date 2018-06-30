#include "Shader.h"

#include <iostream>
#include <fstream>

std::string Shader::readShaderFile(const std::string & filePath)
{
	std::string result;

	std::ifstream file(filePath.c_str());
	std::string line;
	if (file.is_open())
	{
		while (getline(file, line))
		{
			result += line + '\n';
		}

		file.close();
	}

	return result;
}

unsigned int Shader::createShader(const std::string & vertexFilePath, const std::string & fragmentFilePath)
{
	std::string vertexSource = readShaderFile(vertexFilePath);
	std::string fragmentSource = readShaderFile(fragmentFilePath);

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* vsSrc = vertexSource.c_str();
	glShaderSource(vertexShader, 1, &vsSrc, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR: VERTEX COMPILATION FAILED" << infoLog << std::endl;
	}

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fsSrc = fragmentSource.c_str();
	glShaderSource(fragmentShader, 1, &fsSrc, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR: FRAGMENT COMPILATION FAILED" << infoLog << std::endl;
	}

	unsigned int shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);


	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR: SHADER LINKING FAILED" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

Shader::Shader(const std::string& vertexFilePath, const std::string& fragmentFilePath)
{
	id = createShader(vertexFilePath, fragmentFilePath);
}

Shader::~Shader()
{
	glDeleteProgram(id);
}

void Shader::bind()
{
	glUseProgram(id);
}

void Shader::unbind()
{
	glUseProgram(0);
}
