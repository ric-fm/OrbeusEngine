#include "ShaderLoader.h"

#include <fstream>
#include <sstream>

#include <GL/glew.h>

#include "Rendering/Shader.h"
#include "Utils/Log.h"

std::string readShaderFile(const std::string& filePath)
{
	std::string result;

	std::ifstream file(filePath.c_str());
	std::string line;
	if (file.is_open())
	{
		while (getline(file, line))
		{
			std::istringstream lineStream(line);
			std::string element;
			lineStream >> element;

			if (element == "#include")
			{
				lineStream >> element;

				std::string includePath = "Resources/Shaders/" + element;

				std::ifstream file2(includePath.c_str());
				if (file2.is_open())
				{
					while (getline(file2, line))
					{
						result += line + '\n';
					}
				}
				file2.close();
			}
			else
			{
				result += line + '\n';
			}
		}

		file.close();
	}
	else
	{
		Log::error("SHADER. File %s doesn't exists", filePath.c_str());

	}

	return result;
}

Shader* ShaderLoader::loadShader(const std::string& vertexFilePath, const std::string& fragmentFilePath)
{
	Shader* result = new Shader();

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
		Log::error("VERTEX COMPILATION FAILED: %s", infoLog);
	}

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fsSrc = fragmentSource.c_str();
	glShaderSource(fragmentShader, 1, &fsSrc, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		Log::error("FRAGMENT COMPILATION FAILED: %s", infoLog);
	}

	result->id = glCreateProgram();

	glAttachShader(result->id, vertexShader);
	glAttachShader(result->id, fragmentShader);
	glLinkProgram(result->id);


	glGetProgramiv(result->id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(result->id, 512, NULL, infoLog);
		Log::error("SHADER LINKING FAILED: %s", infoLog);

	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return result;;
}
