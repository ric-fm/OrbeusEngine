#include "Shader.h"

#include <GL/glew.h>

#include "Math/Vector3.h"
#include "Math/Vector2.h"
#include "Math/Matrix4.h"
#include "Utils/Log.h"


Shader::Shader()
{
}

Shader::~Shader()
{
	glDeleteProgram(id);
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
