#pragma once

#include <string>
#include <unordered_map>

struct Vector2;
struct Vector3;
struct Vector4;
struct Matrix4;


class Shader
{
	friend class ShaderLoader;
private:
	unsigned int id;
	std::unordered_map<std::string, unsigned int> uniformLocations;

	unsigned int getUniformLocation(const std::string& name);

public:
	Shader();
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
};
