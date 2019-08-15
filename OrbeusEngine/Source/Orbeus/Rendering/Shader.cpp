#include "Shader.h"

#include "Orbeus/Math/Vector2.h"
#include "Orbeus/Math/Vector3.h"
#include "Orbeus/Math/Vector4.h"
#include "Orbeus/Math/Matrix4.h"

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
	: ID(0), vertexPath(vertexPath), fragmentPath(fragmentPath)
{
}

Shader::~Shader()
{
	glDeleteProgram(ID);
}

unsigned int Shader::getUniformLocation(const std::string& name)
{
	if (uniforms.find(name) != uniforms.end())
	{
		return uniforms[name].location;
	}
	return -1;
}

void Shader::registerUniform(const std::string& name, GLDataType type, GLStageType stage, int location)
{
	uniforms[name] = { name, type, stage, location };
}

void Shader::registerAttribute(const std::string& name, GLDataType type, int location)
{
	attributes[name] = { name, type, location };
}

void Shader::bind()
{
	glUseProgram(ID);
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

void Shader::SetFloat4(const std::string& name, float x, float y, float z, float w)
{
	glUniform4f(getUniformLocation(name), x, y, z, w);
}

void Shader::SetFloat4(const std::string& name, const Vector4& vector)
{
	glUniform4f(getUniformLocation(name), vector.x, vector.y, vector.z, vector.w);
}

void Shader::SetMatrix(const std::string& name, const Matrix4& matrix)
{
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix.buffer[0][0]);
}
