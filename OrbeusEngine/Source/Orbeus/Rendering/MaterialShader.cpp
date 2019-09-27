#include "MaterialShader.h"

#include <assert.h>

#include "Orbeus/Rendering/Shader.h"


namespace ORB
{

	Material::Material(const std::string& name, Shader* shader)
		: name(name)
	{
	}

	void Material::addFloatProperty(const std::string& name)
	{
		MaterialProperty* prop = addProperty(name, PropertyType::FLOAT);
		assert(prop != nullptr);
		prop->value = new float(0.0f);
	}

	void Material::addVector2Property(const std::string& name)
	{
		MaterialProperty* prop = addProperty(name, PropertyType::VECTOR2);
		assert(prop != nullptr);
		prop->value = new Vector2();
	}

	void Material::set(const std::string& name, float value)
	{
		MaterialProperty* prop = getProperty(name, PropertyType::FLOAT);
		assert(prop != nullptr);
		*(float*)prop->value = value;
	}

	void Material::set(const std::string& name, const Vector2& vector)
	{
		MaterialProperty* prop = getProperty(name, PropertyType::VECTOR2);
		assert(prop != nullptr);
		Vector2* v = (Vector2*)prop->value;
		*v = vector;
	}

	const float& Material::getFloat(const std::string& name)
	{
		MaterialProperty* prop = getProperty(name, PropertyType::FLOAT);
		assert(prop != nullptr);
		return *(float*)prop->value;
	}

	const Vector2& Material::getVector2(const std::string& name)
	{
		MaterialProperty* prop = getProperty(name, PropertyType::VECTOR2);
		assert(prop != nullptr);
		return *(const Vector2*)prop->value;
	}

	MaterialProperty* Material::getProperty(const std::string& name, PropertyType type)
	{
		return properties.find(name) != properties.end() && properties[name].type == type ? &properties[name] : nullptr;
	}
	MaterialProperty* Material::addProperty(const std::string& name, PropertyType type)
	{
		MaterialProperty* result = nullptr;
		if (properties.find(name) == properties.end())
		{
			properties[name] = MaterialProperty();
			result = &properties[name];
			result->type = type;
		}


		return result;
	}
}
