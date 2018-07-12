#include "Shader.h"

#include <fstream>

#include "Math/Vector3.h"
#include "Math/Vector2.h"
#include "Math/Matrix4.h"
#include "Utils/Log.h"

std::string Shader::readShaderFile(const std::string& filePath)
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
	else
	{
		Log::error("SHADER. File %s doesn't exists", filePath.c_str());

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

	unsigned int shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);


	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		Log::error("SHADER LINKING FAILED: %s", infoLog);

	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

unsigned int Shader::getUniformLocation(const std::string& name)
{
	if (uniformLocations.find(name) != uniformLocations.end())
	{
		return uniformLocations[name];
	}

	int location = glGetUniformLocation(id, name.c_str());
	if (location == -1)
	{
		Log::warning("SHADER. Uniform %s doesn't exists", name.c_str());
	}
	
	uniformLocations[name] = location;

	return location;
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

void Shader::SetBool(const std::string& name, bool value)
{
	glUniform1i(getUniformLocation(name), value);
}

void Shader::SetInt(const std::string& name, int value)
{
	glUniform1i(getUniformLocation(name), value);
}

void Shader::SetFloat(const std::string& name, float value)
{
	glUniform1f(getUniformLocation(name), value);
}

void Shader::SetFloat2(const std::string& name, float x, float y)
{
	glUniform2f(getUniformLocation(name), x, y);
}

void Shader::SetFloat2(const std::string& name, const Vector2& vector)
{
	glUniform2f(getUniformLocation(name), vector.x, vector.y);
}

void Shader::SetFloat3(const std::string& name, float x, float y, float z)
{
	glUniform3f(getUniformLocation(name), x, y, z);
}

void Shader::SetFloat3(const std::string& name, const Vector3& vector)
{
	glUniform3f(getUniformLocation(name), vector.x, vector.y, vector.z);
}

void Shader::SetMatrix(const std::string& name, const Matrix4& matrix)
{
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix.buffer[0][0]);
}
