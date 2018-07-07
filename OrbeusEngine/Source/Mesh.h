#pragma once

#include "GameComponent.h"

#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include "stb_image/stb_image.h"

#include "Vector3.h"
#include "Vector2.h"
#include "String.h"




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

class VertexArray;
class VertexBuffer;
class Texture;

class Mesh : public GameComponent
{
private:
	OBJInfo ParseOBJFile(const std::string& filePath) const;
	OBJIndex ParseOBJIndex(const std::string& index) const;

	MTLInfo ParseMTLFile(const std::string& filePath) const;


	void CreateMesh(const OBJInfo& objInfo);	
	void LoadTextures(MTLInfo& info);

	VertexArray* vertexArray = nullptr;
	VertexBuffer* vertexBuffer = nullptr;
	std::vector<Texture*> textures;

public:
	Mesh(const std::string& filePath);
	~Mesh();

	virtual void render(float deltaTime, Shader* shader) override;

};
