#pragma once

#include <string>
#include <vector>

#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Utils/String.h"


class MeshData;

struct OBJIndex
{
	unsigned int position;
	unsigned int texCoord;
	unsigned int normal;
};

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
	static MeshData* loadMesh(const std::string& filePath);
};
