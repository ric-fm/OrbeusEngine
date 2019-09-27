#pragma once

#include <string>
#include <unordered_map>
#include "Orbeus/Math/Vector2.h"
#include "Orbeus/Math/Vector3.h"
#include "Orbeus/Math/Vector4.h"
#include "Orbeus/Math/Matrix4.h"

namespace ORB
{


	enum class PropertyType
	{
		NONE = 0,
		BOOL, INT, FLOAT,
		VECTOR2, VECTOR3, VECTOR4,
		MATRIX3, MATRIX4
	};

	struct MaterialProperty
	{
		PropertyType type;
		void* value;
	};

	class Shader;

	class Material
	{
	public:
		Material(const std::string& name, Shader* shader);

		void addFloatProperty(const std::string& name);
		void addVector2Property(const std::string& name);

		//void setBool(const std::string& name, bool value);
		//void setInt(const std::string& name, int value);
		void set(const std::string& name, float value);

		void set(const std::string& name, const Vector2& vector);

		const float& getFloat(const std::string& name);
		const Vector2& getVector2(const std::string& name);

		//void setVector3(const std::string& name, const Vector3& vector);

		//void setVector4(const std::string& name, const Vector4& vector);

		////void setMatrix3(const std::string& name, const Matrix3& matrix);
		//void setMatrix4(const std::string& name, const Matrix4& matrix);

		const std::string& getName() const { return name; }
	private:
		MaterialProperty* getProperty(const std::string& name, PropertyType type);
		MaterialProperty* addProperty(const std::string& name, PropertyType type);

		std::string name;

		std::unordered_map<std::string, MaterialProperty> properties;
	};
}