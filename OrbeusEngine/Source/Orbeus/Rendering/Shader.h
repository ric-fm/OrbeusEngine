#pragma once

#include <string>
#include <unordered_map>
#include <GL/glew.h>

struct Vector2;
struct Vector3;
struct Vector4;
struct Matrix4;


enum class GLDataType
{
	NONE = 0,
	BOOL, INT, FLOAT,
	VECTOR2, VECTOR3, VECTOR4,
	MATRIX3, MATRIX4,
	TEXTURE, TEXTURE2D
};

enum class GLStageType
{
	NONE = 0,
	VERTEX,
	PIXEL
};

struct Uniform
{
	std::string name;
	GLDataType type;
	GLStageType stage;
	int location;
};

struct Attribute
{
	std::string name;
	GLDataType type;
	int location;
};

static GLDataType GLIntToGLDataType(GLint type)
{
	switch (type)
	{
	case GL_BOOL:
		return GLDataType::BOOL;
		break;
	case GL_INT:
		return GLDataType::INT;
		break;
	case GL_FLOAT:
		return GLDataType::FLOAT;
		break;
	case GL_FLOAT_VEC2:
		return GLDataType::VECTOR2;
		break;
	case GL_FLOAT_VEC3:
		return GLDataType::VECTOR3;
		break;
	case GL_FLOAT_VEC4:
		return GLDataType::VECTOR4;
		break;
	case GL_FLOAT_MAT3:
		return GLDataType::MATRIX3;
		break;
	case GL_FLOAT_MAT4:
		return GLDataType::MATRIX4;
		break;
	case GL_SAMPLER:
		return GLDataType::TEXTURE;
		break;
	case GL_SAMPLER_2D:
		return GLDataType::TEXTURE2D;
		break;
	default:
		return GLDataType::NONE;
		break;
	}
}

static std::string GLDataTypeToString(GLDataType type)
{
	switch (type)
	{
	case GLDataType::NONE:
		return "NONE";
		break;
	case GLDataType::BOOL:
		return "BOOL";
		break;
	case GLDataType::INT:
		return "INT";
		break;
	case GLDataType::FLOAT:
		return "FLOAT";
		break;
	case GLDataType::VECTOR2:
		return "VECTOR2";
		break;
	case GLDataType::VECTOR3:
		return "VECTOR3";
		break;
	case GLDataType::VECTOR4:
		return "VECTOR4";
		break;
	case GLDataType::MATRIX3:
		return "MATRIX3";
		break;
	case GLDataType::MATRIX4:
		return "MATRIX4";
		break;
	case GLDataType::TEXTURE2D:
		return "TEXTURE2D";
		break;
	default:
		return "NONE";
		break;
	}
}

class Shader
{
	friend class ShaderLoader;

public:
	Shader(const std::string& vertexPath, const std::string& fragmentPath);
	~Shader();

	void bind();
	void unbind();

	void SetBool(const std::string& name, bool value);

	void SetInt(const std::string& name, int value);

	void SetFloat(const std::string& name, float value);
	
	void SetFloat2(const std::string& name, float x, float y);
	void SetFloat2(const std::string& name, const Vector2& vector);

	void SetFloat3(const std::string& name, float x, float y, float z);
	void SetFloat3(const std::string& name, const Vector3& vector);

	void SetFloat4(const std::string& name, float x, float y, float z, float w);
	void SetFloat4(const std::string& name, const Vector4& vector);

	void SetMatrix(const std::string& name, const Matrix4& matrix);

	const std::unordered_map<std::string, Attribute>& getAttributes() const { return attributes; }
	const std::unordered_map<std::string, Uniform>& getUniforms() const { return uniforms; }

private:
	void registerAttribute(const std::string& name, GLDataType type, int location);
	void registerUniform(const std::string& name, GLDataType type, GLStageType stage, int location);
	unsigned int getUniformLocation(const std::string& name);

	unsigned int ID;

	std::string vertexPath;
	std::string fragmentPath;
	std::unordered_map<std::string, Attribute> attributes;
	std::unordered_map<std::string, Uniform> uniforms;
};
