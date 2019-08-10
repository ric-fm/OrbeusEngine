#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "Orbeus/Math/Vector2.h"
#include "Orbeus/Math/Vector3.h"
#include "Orbeus/Utils/String.h"
#include "Orbeus/Utils/Hash.h"


class MeshData;

struct OBJIndex
{
	unsigned int position;
	unsigned int texCoord;
	unsigned int normal;

	bool operator==(const OBJIndex &other) const
	{
		return (position == other.position
			&& texCoord == other.texCoord
			&& normal == other.normal);
	}
};

// https://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key
namespace std
{
	template<>
	struct hash<OBJIndex>
	{
		size_t operator()(OBJIndex const& index) const noexcept
		{
			std::size_t result = 0;
			hash_combine(result, index.position);
			hash_combine(result, index.texCoord);
			hash_combine(result, index.normal);
			return result;
		}
	};
}

struct OBJInfo
{
	std::string name;
	PathInfo pathInfo;
	std::vector<Vector3> positions;
	std::vector<Vector3> normals;
	std::vector<Vector2> texCoords;
	std::vector<OBJIndex> indices;
	std::string mtl;
};

struct TextureInfo
{
	std::string name;
	std::string type;
};

struct MaterialInfo
{
	std::string name;
	std::vector<TextureInfo*> textures;
	Vector3 diffuse;
	Vector3 specular;
};

struct MTLInfo
{
	PathInfo pathInfo;
	std::vector<MaterialInfo*> materials;
};

struct Vertex
{
	Vector3 position;
	Vector3 normal;
	Vector2 texCoord;
};

class MeshLoader
{
public:
	static MeshData* loadMesh(const std::string& filePath, bool indexed = true);
};
