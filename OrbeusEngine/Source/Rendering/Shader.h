#pragma once

#include <string>
#include <unordered_map>

#include <GL/glew.h>

struct Vector3;
struct Vector2;
struct Matrix4;


class Shader
{
private:
	unsigned int id;
	std::unordered_map<std::string, unsigned int> uniformLocations;

	std::string readShaderFile(const std::string& filePath);
	unsigned int createShader(const std::string& vertexFilePath, const std::string& fragmentFilePath);

	unsigned int getUniformLocation(const std::string& name);

public:
	Shader(const std::string& vertexFilePath, const std::string& fragmentFilePath);
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

	void SetMatrix(const std::string& name, const Matrix4& matrix);
};